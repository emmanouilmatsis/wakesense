/****************** (C) COPYRIGHT 2009 VectorNav Technologies ******************
* File Name          : main.c
* Author             : John Brashear
* Version            : V1.0.0
* Date               : 07/14/2009
* Description        : The purpose of this program is to demonstrate a sucessful
*                      SPI transaction with the VN-100 module. The program when
*                      installed on the STM32F10B evaluation board will poll the
*                      Yaw, Pitch, and Roll register from the VN-100 at a
*                      frequency of 10Hz.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, VECTORNAV SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
* CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF
* SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION 
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "SysClock.h"
#include "VN_lib.h"
#include <stdio.h>

/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DELAY							1e3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

VN100_SPI_Packet* ReturnPacket;
char model[12]; /* Sensor model */
unsigned long int revision; /* Hardware revision */
unsigned long int serial[3]; /* Serial number */
unsigned long int version; /* Firmware version */
VN100_BaudType baud; /* Serial baud rate */
VN100_ADORType ador; /* Serial Async Data Output Register */
VN100_ADOFType adof; /* Serial Async Data Output Frequency */
float ypr[3]; /* yaw, pitch, roll */
float q[4]; /* quaterion */
float mag[3]; /* magnetic */
float acc[3]; /* acceleration */
float gyr[3]; /* angular rates */

/* Directional Cosine Matrix data structure */
float dcm_data[9] = {0};
float *dcm_ptr[3] = {&dcm_data[0], &dcm_data[3], &dcm_data[6]};
float **dcm = dcm_ptr;

float sig[10]; /* Tuning parameters */
float hsi[12]; /* Hard/Soft Iron */
float act[12]; /* Accelerometer compensation */
float ref[9]; /* Reference vectors */
float rfr[12]; /* Reference frame rotation */
float atp[4]; /* Automatic tuning parameters */
VN100_AccGainType acg; /* Accelerometer gain */

/* Quaternions */
float q1[4], q2[4], q3[4];

/* Directional Cosine Matrix data structure */
float dcm2_data[9] = {0};
float *dcm2_ptr[3] = {&dcm2_data[0], &dcm2_data[3], &dcm2_data[6]};
float **dcm2 = dcm2_ptr;

/* SPI Initialization Structure */
SPI_InitTypeDef   SPI_InitStructure;

/* UART Initialization Structure */
USART_InitTypeDef USART_InitStructure;


ErrorStatus HSEStartUpStatus;

typedef struct SPITestType
{
	char title[128];
	unsigned long  bytes;
	unsigned char  TX[128];
	unsigned char  RX[128];
} SPITestType;

/* Support functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void Serial_SendByte(char byte);
void Serial_SendHexBytes(u32 bytes, u8 *buffer);
int fputc(int ch, FILE *f);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	u32 size;
	u8 reg;


#ifdef DEBUG
  debug();
#endif

  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();
	
	/* Configure SysClock */
	
/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);  
  
	/* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);

  /* 1st phase: SPI1 Master and SPI2 Slave */
  /* SPI1 Config -------------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
	
	/* Enable SPI1 */
  SPI_Cmd(SPI1, ENABLE);
	
	/* Delay 1s */
	VN_Delay(1e6);
	
	printf("\n\n------- Starting SPI Test Script -------\n\n");
	
	/* Model Number */
	size = 12*4+4;
	reg = 1;
	printf("Read Register %u - Model\n", reg);	
	ReturnPacket = VN100_SPI_GetModel(0, model);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Model: %s\n\n", model);
	
	VN_Delay(DELAY);
	
	/* Hardware Revision */
	size = 4+4;
	reg = 2;
	printf("Read Register %u - Hardware Revision\n", reg);	
	ReturnPacket = VN100_SPI_GetHWRev(0, &revision);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Hardware Revision: %u\n\n", revision);
	
	VN_Delay(DELAY);
		
	/* Model Number */
	size = 12*4+4;
	reg = 3;
	printf("Read Register %u - Model Number\n", reg);	
	ReturnPacket = VN100_SPI_GetSerial(0, serial);
	printf("Model Number: %08X%08X%08X\n\n", serial[0], serial[1], serial[2]);
	
	VN_Delay(DELAY);
		
	/* Firmware version */
	size = 4+4;
	reg = 4;
	printf("Read Register %u - Firmware version\n", reg);	
	ReturnPacket = VN100_SPI_GetFWVer(0, &version);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Firmware Version: %u\n\n", version);	
	
	VN_Delay(DELAY);
		
	/* Serial Baud Rate */
	size = 4+4;
	reg = 5;
	printf("Read Register %u - Serial Baud Rate\n", reg);
	ReturnPacket = VN100_SPI_GetBaudRate(0, &baud);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Serial Baud Rate: %u\n\n", baud);
	
	VN_Delay(DELAY);
		
	/* Async Data Output Type */
	size = 4+4;
	reg = 6;
	printf("Read Register %u - Async Data Output Type\n", reg);
	ReturnPacket = VN100_SPI_GetADOR(0, &ador);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Async Data Output Type: %u\n\n", ador);
	
	VN_Delay(DELAY);
		
	/* Async Data Output Frequency */
	size = 4+4;
	reg = 7;
	printf("Read Register %u - Async Data Output Frequency\n", reg);
	ReturnPacket = VN100_SPI_GetADOF(0, &adof);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Async Data Output Frequency: %u\n\n", adof);
	
	
	VN_Delay(DELAY);
		
	/* Attitude (Yaw, Pitch, Roll) */
	size = 3*4+4;
	reg = 8;
	printf("Read Register %u - Attitude (Yaw, Pitch, Roll)\n", reg);
	ReturnPacket = VN100_SPI_GetYPR(0, &ypr[0], &ypr[1], &ypr[2]);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Yaw: %+07.2f\nPitch: %+07.2f\nRoll: %+07.2f\n\n", ypr[0], ypr[1], ypr[2]);
	
	VN_Delay(DELAY);
			
	/* Attitude (Quaternion) */
	size = 4*4+4;
	reg = 9;
	printf("Read Register %u - Attitude (Quaternion)\n", reg);
	ReturnPacket = VN100_SPI_GetQuat(0, q);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n\n", q[0], q[1], q[2], q[3]);
	
	VN_Delay(DELAY);
		
	/* Quaternion and Magnetic */
	size = 7*4+4;
	reg = 10;
	printf("Read Register %u - Quaternion and Magnetic\n", reg);
	ReturnPacket = VN100_SPI_GetQuatMag(0, q, mag);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n", q[0], q[1], q[2], q[3]);
	printf("MX: %+07.4f\nMY: %+07.4f\nMZ: %+07.4f\n\n",  mag[0], mag[1], mag[2]);	
	
	VN_Delay(DELAY);
		
	/* Quaternion and Acceleration */
	size = 7*4+4;
	reg = 11;
	printf("Read Register %u - Quaternion and Acceleration\n", reg);
	ReturnPacket = VN100_SPI_GetQuatAcc(0, q, acc);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n", q[0], q[1], q[2], q[3]);
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n\n",  acc[0], acc[1], acc[2]);	
	
	VN_Delay(DELAY);
		
	/* Quaternion and Angular Rates */
	size = 7*4+4;
	reg = 12;
	printf("Read Register %u - Quaternion and Angular Rates\n", reg);
	ReturnPacket = VN100_SPI_GetQuatRates(0, q, gyr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n", q[0], q[1], q[2], q[3]);
	printf("GX: %+08.4f\nGY: %+08.4f\nGZ: %+08.4f\n\n", gyr[0], gyr[1], gyr[2]);	
	
	VN_Delay(DELAY);
		
	/* Quaternion, Magnetic and Acceleration */
	size = 10*4+4;
	reg = 13;
	printf("Read Register %u - Quaternion, Magnetic and Acceleration\n", reg);
	ReturnPacket = VN100_SPI_GetQuatMagAcc(0, q, mag, acc);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n", q[0], q[1], q[2], q[3]);
	printf("MX: %+07.4f\nMY: %+07.4f\nMZ: %+07.4f\n\n",  mag[0], mag[1], mag[2]);
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n\n",  acc[0], acc[1], acc[2]);
	
	VN_Delay(DELAY);
		
	/* Quaternion, Acceleration and Angular Rates */
	size = 10*4+4;
	reg = 14;
	printf("Read Register %u - Quaternion, Acceleration and Angular Rates\n", reg);
	ReturnPacket = VN100_SPI_GetQuatAccRates(0, q, acc, gyr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n", q[0], q[1], q[2], q[3]);
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n\n",  acc[0], acc[1], acc[2]);
	printf("GX: %+08.4f\nGY: %+08.4f\nGZ: %+08.4f\n\n", gyr[0], gyr[1], gyr[2]);	
	
	VN_Delay(DELAY);
		
	/* Quaternion, Magnetic, Acceleration and Angular Rates */
	size = 13*4+4;
	reg = 15;
	printf("Read Register %u - Quaternion, Magnetic, Acceleration and Angular Rates\n", reg);
	ReturnPacket = VN100_SPI_GetQuatMagAccRates(0, q, mag, acc, gyr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Q1: %+09.6f\nQ2: %+09.6f\nQ3: %+09.6f\nQ4: %+09.6f\n", q[0], q[1], q[2], q[3]);
	printf("MX: %+07.4f\nMY: %+07.4f\nMZ: %+07.4f\n\n",  mag[0], mag[1], mag[2]);	
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n\n",  acc[0], acc[1], acc[2]);
	printf("GX: %+08.4f\nGY: %+08.4f\nGZ: %+08.4f\n\n", gyr[0], gyr[1], gyr[2]);	
	
	VN_Delay(DELAY);
		
	/* Attitude (Directional Cosine Matrix) */
	size = 9*4+4;
	reg = 16;
	printf("Read Register %u - Attitude (Directional Cosine Matrix)\n", reg);
	ReturnPacket = VN100_SPI_GetDCM(0, dcm);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("C11: %+13.6E\nC12: %+13.6E\nC13: %+13.6E\n", dcm[0][0], dcm[0][1], dcm[0][2]);
	printf("C21: %+13.6E\nC22: %+13.6E\nC23: %+13.6E\n", dcm[1][0], dcm[1][1], dcm[1][2]);
	printf("C31: %+13.6E\nC23: %+13.6E\nC33: %+13.6E\n\n", dcm[2][0], dcm[2][1], dcm[2][2]);
	
	VN_Delay(DELAY);
		
	/* Magnetic Measurements */
	size = 3*4+4;
	reg = 17;
	printf("Read Register %u - Magnetic Measurements\n", reg);
	ReturnPacket = VN100_SPI_GetMag(0, mag);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("MX: %+07.4f\nMY: %+07.4f\nMZ: %+07.4f\n\n",  mag[0], mag[1], mag[2]);
	
	VN_Delay(DELAY);
		
	/* Acceleration Measurements */
	size = 3*4+4;
	reg = 18;
	printf("Read Register %u - Acceleration Measurements\n", reg);
	ReturnPacket = VN100_SPI_GetAcc(0, acc);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n\n",  acc[0], acc[1], acc[2]);
		
	VN_Delay(DELAY);
		
	/* Angular Rate Measurements */
	size = 3*4+4;
	reg = 19;
	printf("Read Register %u - Angular Rate Measurements\n", reg);
	ReturnPacket = VN100_SPI_GetRates(0, gyr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("GX: %+08.4f\nGY: %+08.4f\nGZ: %+08.4f\n\n", gyr[0], gyr[1], gyr[2]);
		
	VN_Delay(DELAY);	
	
	/* Magnetic, Acceleration and Angular Rates */
	size = 9*4+4;
	reg = 20;
	printf("Read Register %u - Magnetic, Acceleration and Angular Rates\n", reg);
	ReturnPacket = VN100_SPI_GetMagAccRates(0, mag, acc, gyr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("MX: %+07.4f\nMY: %+07.4f\nMZ: %+07.4f\n\n",  mag[0], mag[1], mag[2]);
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n\n",  acc[0], acc[1], acc[2]);
	printf("GX: %+08.4f\nGY: %+08.4f\nGZ: %+08.4f\n\n", gyr[0], gyr[1], gyr[2]);
		
	VN_Delay(DELAY);	
	
	/* Magnetic and Gravity Reference */
	size = 6*4+4;
	reg = 21;
	printf("Read Register %u - Magnetic and Gravity References\n", reg);
	ReturnPacket = VN100_SPI_GetMagAccRef(0, mag, acc);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("MRX: %+13.6E\nMRY: %+13.6E\nMRZ: %+13.6E\n",  mag[0], mag[1], mag[2]);
	printf("GRX: %+13.6E\nGRY: %+13.6E\nGRZ: %+13.6E\n\n",  acc[0], acc[1], acc[2]);
		
	VN_Delay(DELAY);	
	
	/* Filter Measurements Variance Parameters */
	size = 10*4+4;
	reg = 22;
	printf("Read Register %u - Filter Measurements Variance Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetFiltMeasVar(0, sig);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("R1: %+13.6E\nR2: %+13.6E\nR3: %+13.6E\n",  sig[0], sig[1], sig[2]);
	printf("R4: %+13.6E\nR5: %+13.6E\nR6: %+13.6E\n",  sig[3], sig[4], sig[5]);
	printf("R7: %+13.6E\nR8: %+13.6E\nR9: %+13.6E\n",  sig[6], sig[7], sig[8]);
	printf("R10: %+13.6E\n\n", sig[9]);
		
	VN_Delay(DELAY);	
	
	/* Magnetic Hard/Soft Iron Compensation Parameters */
	size = 12*4+4;
	reg = 23;
	printf("Read Register %u - Magnetic Hard/Soft Iron Compensation Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetHardSoftIronComp(0, hsi);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("H01: %+13.6E\nH02: %+13.6E\nH03: %+13.6E\n",  hsi[0], hsi[1], hsi[2]);
	printf("H04: %+13.6E\nH05: %+13.6E\nH06: %+13.6E\n",  hsi[3], hsi[4], hsi[5]);
	printf("H07: %+13.6E\nH08: %+13.6E\nH09: %+13.6E\n",  hsi[6], hsi[7], hsi[8]);
	printf("H10: %+13.6E\nH11: %+13.6E\nH12: %+13.6E\n\n",  hsi[9], hsi[10], hsi[11]);
	
	VN_Delay(DELAY);	
	
	/* Disturbance Tuning Parameters - Read */
	size = 4*4+4;
	reg = 24;
	printf("Read Register %u - Filter Active Tuning Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetFiltActTuning(0, &atp[0], &atp[1], &atp[2], &atp[3]);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("T1: %+13.6E\nT2: %+13.6E\nT3: %+13.6E\nT4: %+13.6E\n\n",  atp[0], atp[1], atp[2], atp[3]);
	
	VN_Delay(DELAY);
	
	/* Accelerometer Compensation Parameters - Read */
	size = 12*4+4;
	reg = 25;
	printf("Read Register %u - Accelerometer Compensation Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetAccComp(0, act);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("A01: %+13.6E\nA02: %+13.6E\nA03: %+13.6E\n",  act[0], act[1], act[2]);
	printf("A04: %+13.6E\nA05: %+13.6E\nA06: %+13.6E\n",  act[3], act[4], act[5]);
	printf("A07: %+13.6E\nA08: %+13.6E\nA09: %+13.6E\n",  act[6], act[7], act[8]);
	printf("A10: %+13.6E\nA11: %+13.6E\nA12: %+13.6E\n\n",  act[9], act[10], act[11]);
		
	VN_Delay(DELAY);
	
	/* Reference Frame Rotation - Read */
	size = 12*4+4;
	reg = 26;
	printf("Read Register %u - Reference Frame Rotation\n", reg);
	ReturnPacket = VN100_SPI_GetRefFrameRot(0, rfr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("R01: %+13.6E\nR02: %+13.6E\nR03: %+13.6E\n",  rfr[0], rfr[1], rfr[2]);
	printf("R04: %+13.6E\nR05: %+13.6E\nR06: %+13.6E\n",  rfr[3], rfr[4], rfr[5]);
	printf("R07: %+13.6E\nR08: %+13.6E\nR09: %+13.6E\n",  rfr[6], rfr[7], rfr[8]);
	printf("R10: %+13.6E\nR11: %+13.6E\nR12: %+13.6E\n\n",  rfr[9], rfr[10], rfr[11]);	
	
	VN_Delay(DELAY);
	
	
	/* Yaw, Pitch, Roll, Magnetic, Acceleration, Angular Rates - Read */
	size = 12*4+4;
	reg = 27;
	printf("Read Register %u - Yaw, Pitch, Roll, Magnetic, Acceleration, Angular Rates\n", reg);
	ReturnPacket = VN100_SPI_GetYPRMagAccRates(0, ypr, mag, acc, gyr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf(" Y: %+07.2f\n P: %+07.2f\n R: %+07.2f\n",  ypr[0], ypr[1], ypr[2]);
	printf("MX: %+07.4f\nMY: %+07.4f\nMZ: %+07.4f\n",  mag[0], mag[1], mag[2]);
	printf("AX: %+07.3f\nAY: %+07.3f\nAZ: %+07.3f\n",  acc[0], acc[1], acc[2]);
	printf("GX: %+08.4f\nGY: %+08.4f\nGZ: %+08.4f\n\n",  gyr[0], gyr[1], gyr[2]);
		
	VN_Delay(DELAY);
	
	/* Accelerometer Gain - Read */
	size = 1*4+4;
	reg = 28;
	printf("Read Register %u - Accelerometer Gain\n", reg);
	ReturnPacket = VN100_SPI_GetAccGain(0, &acg);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("A: %u\n\n",  acg);
	
	VN_Delay(DELAY);
		


	
	/* Serial Baud Rate - Write */
	size = 4+4;
	reg = 5;
	printf("Write Register %u - Serial Baud Rate\n", reg);
	ReturnPacket = VN100_SPI_SetBaudRate(0, baud);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Serial Baud Rate: %u\n\n", baud);
	
	VN_Delay(1e5);
		
	/* Serial Baud Rate - Read */
	size = 4+4;
	reg = 5;
	printf("Read Register %u - Serial Baud Rate\n", reg);
	ReturnPacket = VN100_SPI_GetBaudRate(0, &baud);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Serial Baud Rate: %u\n\n", baud);
	
	VN_Delay(DELAY);
		
	/* Async Data Output Register - Write */
	size = 4+4;
	reg = 6;
	printf("Write Register %u - Async Data Output Type\n", reg);
	ReturnPacket = VN100_SPI_SetADOR(0, ador);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Async Data Output Type: %u\n\n", ador);
	
	VN_Delay(1e5);
		
	/* Async Data Output Register - Read */
	size = 4+4;
	reg = 6;
	printf("Read Register %u - Async Data Output Type\n", reg);
	ReturnPacket = VN100_SPI_GetADOR(0, &ador);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Async Data Output Type: %u\n\n", ador);
	
	VN_Delay(DELAY);
		
	/* Async Data Output Frequency - Write */
	size = 4+4;
	reg = 7;
	printf("Write Register %u - Async Data Output Freq\n", reg);
	ReturnPacket = VN100_SPI_SetADOF(0, adof);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Async Data Output Type: %u\n\n", adof);
	
	VN_Delay(1e5);
		
	/* Async Data Output Frequency - Read */
	size = 4+4;
	reg = 7;
	printf("Read Register %u - Async Data Output Freq\n", reg);
	ReturnPacket = VN100_SPI_GetADOF(0, &adof);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("Async Data Output Type: %u\n\n", adof);
	
	VN_Delay(DELAY);
		
	/* Magnetic and Gravity Reference - Write */
	size = 6*4+4;
	reg = 21;
	printf("Read Register %u - Magnetic and Gravity References\n", reg);
	ReturnPacket = VN100_SPI_SetMagAccRef(0, mag, acc);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("MRX: %+13.6E\nMRY: %+13.6E\nMRZ: %+13.6E\n",  mag[0], mag[1], mag[2]);
	printf("GRX: %+13.6E\nGRY: %+13.6E\nGRZ: %+13.6E\n\n",  acc[0], acc[1], acc[2]);
	
	VN_Delay(DELAY);
		
	/* Magnetic and Gravity Reference - Read */
	size = 6*4+4;
	reg = 21;
	printf("Read Register %u - Magnetic and Gravity References\n", reg);
	ReturnPacket = VN100_SPI_GetMagAccRef(0, mag, acc);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("MRX: %+13.6E\nMRY: %+13.6E\nMRZ: %+13.6E\n",  mag[0], mag[1], mag[2]);
	printf("GRX: %+13.6E\nGRY: %+13.6E\nGRZ: %+13.6E\n\n",  acc[0], acc[1], acc[2]);
	
	VN_Delay(DELAY);	

	/* Filter Measurement Variance Parameters - Write */
	size = 10*4+4;
	reg = 22;
	printf("Write Register %u - Filter Measurement Variance Parameters\n", reg);
	sig[0] = 1e-9;
	sig[1] = 1e-9;
	sig[2] = 1e-9;
	sig[3] = 1e-9;
	sig[4] = 2e-6;
	sig[5] = 2e-6;
	sig[6] = 2e-6;
	sig[7] = 5e-5;
	sig[8] = 5e-5;
	sig[9] = 5e-5;	
	ReturnPacket = VN100_SPI_SetFiltMeasVar(0, sig);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf(" V1: %+13.6E\n V2: %+13.6E\n V3: %+13.6E\n",  sig[0], sig[1], sig[2]);
	printf(" V4: %+13.6E\n V5: %+13.6E\n V6: %+13.6E\n",  sig[3], sig[4], sig[5]);
	printf(" V7: %+13.6E\n V8: %+13.6E\n V9: %+13.6E\n",  sig[6], sig[7], sig[8]);
	printf("V10: %+13.6E\n\n",  sig[9]);
	
	VN_Delay(DELAY);
		
	/* Filter Measurement Variance Parameters - Read */
	size = 10*4+4;
	reg = 22;
	printf("Read Register %u - Filter Measurement Variance Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetFiltMeasVar(0, sig);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf(" V1: %+13.6E\n V2: %+13.6E\n V3: %+13.6E\n",  sig[0], sig[1], sig[2]);
	printf(" V4: %+13.6E\n V5: %+13.6E\n V6: %+13.6E\n",  sig[3], sig[4], sig[5]);
	printf(" V7: %+13.6E\n V8: %+13.6E\n V9: %+13.6E\n",  sig[6], sig[7], sig[8]);
	printf("V10: %+13.6E\n\n",  sig[9]);
	
	VN_Delay(DELAY);	

	/* Magnetic Hard/Soft Iron Compensation Parameters - Write */
	size = 12*4+4;
	reg = 23;
	printf("Write Register %u - Magnetic Hard/Soft Iron Compensation Parameters\n", reg);
	hsi[0] = 1.0;
	hsi[1] = 0.01;
	hsi[2] = 0.01;
	hsi[3] = -0.02;
	hsi[4] = 1.0;
	hsi[5] = 0.0;
	hsi[6] = -0.1;
	hsi[7] = 0.1;
	hsi[8] = 1.0;
	hsi[9] = 0.2;
	hsi[10] = 0.1;
	hsi[11] = -0.1;
	ReturnPacket = VN100_SPI_SetHardSoftIronComp(0, hsi);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf(" H1: %+13.6E\n H2: %+13.6E\n H3: %+13.6E\n",  hsi[0], hsi[1], hsi[2]);
	printf(" H4: %+13.6E\n H5: %+13.6E\n H6: %+13.6E\n",  hsi[3], hsi[4], hsi[5]);
	printf(" H7: %+13.6E\n H8: %+13.6E\n H9: %+13.6E\n",  hsi[6], hsi[7], hsi[8]);
	printf("H10: %+13.6E\nH11: %+13.6E\nH12: %+13.6E\n",  hsi[9], hsi[10], hsi[11]);
	
	VN_Delay(DELAY);
		
	/* Magnetic Hard/Soft Iron Compensation Parameters */
	size = 12*4+4;
	reg = 23;
	printf("Read Register %u - Magnetic Hard/Soft Iron Compensation Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetHardSoftIronComp(0, hsi);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf(" H1: %+13.6E\n H2: %+13.6E\n H3: %+13.6E\n",  hsi[0], hsi[1], hsi[2]);
	printf(" H4: %+13.6E\n H5: %+13.6E\n H6: %+13.6E\n",  hsi[3], hsi[4], hsi[5]);
	printf(" H7: %+13.6E\n H8: %+13.6E\n H9: %+13.6E\n",  hsi[6], hsi[7], hsi[8]);
	printf("H10: %+13.6E\nH11: %+13.6E\nH12: %+13.6E\n",  hsi[9], hsi[10], hsi[11]);	

	VN_Delay(DELAY);
	
	/* Filter Active Tuning Parameters - Write */
	size = 4*4+4;
	reg = 24;
	printf("Write Register %u - Filter Active Tuning Parameters\n", reg);
	atp[0] = 1.0;
	atp[1] = 1.0;
	atp[2] = 0.99;
	atp[3] = 0.99;
	ReturnPacket = VN100_SPI_SetFiltActTuning(0, atp[0], atp[1], atp[2], atp[3]);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("T1: %+13.6E\nT2: %+13.6E\nT3: %+13.6E\nT4: %+13.6E\n\n",  atp[0], atp[1], atp[2], atp[3]);
	
	VN_Delay(DELAY);
	
	/* Filter Active Tuning Parameters - Read */
	size = 4*4+4;
	reg = 24;
	printf("Read Register %u - Filter Active Tuning Parameters\n", reg);
	ReturnPacket = VN100_SPI_GetFiltActTuning(0, &atp[0], &atp[1], &atp[2], &atp[3]);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("T1: %+13.6E\nT2: %+13.6E\nT3: %+13.6E\nT4: %+13.6E\n\n",  atp[0], atp[1], atp[2], atp[3]);
		
	VN_Delay(DELAY);
	
	
	/* Accelerometer Compensation - Write */
	size = 12*4+4;
	reg = 25;
	printf("Write Register %u - Accelerometer Compensation\n", reg);
	ReturnPacket = VN100_SPI_SetAccComp(0, act);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("A01: %+13.6E\nA02: %+13.6E\nA03: %+13.6E\n",  act[0], act[1], act[2]);
	printf("A04: %+13.6E\nA05: %+13.6E\nA06: %+13.6E\n",  act[3], act[4], act[5]);
	printf("A07: %+13.6E\nA08: %+13.6E\nA09: %+13.6E\n",  act[6], act[7], act[8]);
	printf("A10: %+13.6E\nA11: %+13.6E\nA12: %+13.6E\n\n",  act[9], act[10], act[11]);
	
	VN_Delay(DELAY);
	
	/* Accelerometer Compensation - Read */
	size = 12*4+4;
	reg = 25;
	printf("Read Register %u - Accelerometer Compensation\n", reg);
	ReturnPacket = VN100_SPI_GetAccComp(0, act);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("A01: %+13.6E\nA02: %+13.6E\nA03: %+13.6E\n",  act[0], act[1], act[2]);
	printf("A04: %+13.6E\nA05: %+13.6E\nA06: %+13.6E\n",  act[3], act[4], act[5]);
	printf("A07: %+13.6E\nA08: %+13.6E\nA09: %+13.6E\n",  act[6], act[7], act[8]);
	printf("A10: %+13.6E\nA11: %+13.6E\nA12: %+13.6E\n\n",  act[9], act[10], act[11]);
		
	VN_Delay(DELAY);
	
	
	/* Reference Frame Rotation - Write */
	size = 9*4+4;
	reg = 26;
	printf("Write Register %u - Reference Frame Rotation\n", reg);
	rfr[0] = 1.0;
	rfr[1] = 0.01;
	rfr[2] = 0.01;
	rfr[3] = -0.02;
	rfr[4] = 1.0;
	rfr[5] = 0.0;
	rfr[6] = -0.1;
	rfr[7] = 0.1;
	rfr[8] = 1.0;
	ReturnPacket = VN100_SPI_SetRefFrameRot(0, rfr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("R01: %+13.6E\nR02: %+13.6E\nR03: %+13.6E\n",  rfr[0], rfr[1], rfr[2]);
	printf("R04: %+13.6E\nR05: %+13.6E\nR06: %+13.6E\n",  rfr[3], rfr[4], rfr[5]);
	printf("R07: %+13.6E\nR08: %+13.6E\nR09: %+13.6E\n\n",  rfr[6], rfr[7], rfr[8]);
		
	VN_Delay(DELAY);
		
	/* Reference Frame Rotation - Read */
	size = 9*4+4;
	reg = 26;
	printf("Read Register %u - Reference Frame Rotation\n", reg);
	ReturnPacket = VN100_SPI_GetRefFrameRot(0, rfr);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("R01: %+13.6E\nR02: %+13.6E\nR03: %+13.6E\n",  rfr[0], rfr[1], rfr[2]);
	printf("R04: %+13.6E\nR05: %+13.6E\nR06: %+13.6E\n",  rfr[3], rfr[4], rfr[5]);
	printf("R07: %+13.6E\nR08: %+13.6E\nR09: %+13.6E\n\n",  rfr[6], rfr[7], rfr[8]);
		
	VN_Delay(DELAY);
	
	/* Accelerometer Gain - Write */
	size = 1*4+4;
	reg = 28;
	acg = VN100_AccGain_6G;
	printf("Write Register %u - Accelerometer Gain\n", reg);
	ReturnPacket = VN100_SPI_SetAccGain(0, acg);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("A: %u\n\n",  acg);
	
	VN_Delay(DELAY);	
	
	/* Accelerometer Gain - Read */
	size = 1*4+4;
	reg = 28;
	printf("Read Register %u - Accelerometer Gain\n", reg);
	ReturnPacket = VN100_SPI_GetAccGain(0, &acg);
	Serial_SendHexBytes(size, (u8*)ReturnPacket);
	printf("A: %u\n\n",  acg);
	
	VN_Delay(DELAY);	
	
	/* Restore factory default settings */
	VN100_SPI_RestoreFactorySettings(0);
		
	VN_Delay(DELAY);  
  
  /* Infinite loop */
  while (1)
  {
	for(size=0;size<10;size++){
    	VN_Delay(1e6);

		/* Get YMR */
      	VN100_SPI_GetYPRMagAccRates(0, ypr, mag, acc, gyr);
      
      	/* Display ypr */
      	printf("Yaw: %+07.2f\nPitch: %+07.2f\nRoll: %+07.2f\n\n", ypr[0], ypr[1], ypr[2]);
	}
  }
}

/*******************************************************************************
* Function Name  : fputc
* Description    : Redirects data out serial port
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}

/*******************************************************************************
* Function Name  : Serial_SendByte(u8 byte)
* Description    : Send single byte over serial                                        
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Serial_SendByte(char byte){
	USART_SendData(USART1, byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* Function Name  : Serial_SendHexBytes(u32 bytes, u8 *buffer)
* Description    : Send the given number of bytes out the serial as hexadecimal
*                  with spaces in between bytes.                                        
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Serial_SendHexBytes(u32 bytes, u8 *buffer)
{
	/* Send out buffer over serial */
	char hexbyte[3];	
	u32 RxIdx = 0;
	
	while(RxIdx < bytes){
		/* Get string hex for byte */
		sprintf(hexbyte, "%02X", buffer[RxIdx++]);
		
		/* Send hex characters */
		Serial_SendByte(hexbyte[0]);
		Serial_SendByte(hexbyte[1]);
		Serial_SendByte(0x20);	/* space */
	}
	
	Serial_SendByte(13);
	Serial_SendByte(10);
}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

  /* Enable peripheral clocks --------------------------------------------------*/
  /* GPIOA, GPIOB, SPI1, USART1, and AFIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_SPI1| RCC_APB2Periph_AFIO, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Configure SPI1 pins: CS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
  /* Configure USART1 pins: TX ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Configure USART1 pins: RX ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}




#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/**************** (C) COPYRIGHT 2009 VectorNav Technologies *****END OF FILE****/
