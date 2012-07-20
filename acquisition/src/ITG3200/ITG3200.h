#include "Arduino.h"

#ifndef ITG3200_h
#define ITG3200_h

/* ------- Register names ------- */
#define ITG3200_WHO_AM_I 0x00
#define ITG3200_SMPLRT_DIV 0x15
#define ITG3200_DLPF_FS 0x16
#define ITG3200_INT_CFG 0x17
#define ITG3200_INT_STATUS 0x1A
#define ITG3200_TEMP_OUT_H 0x1B
#define ITG3200_TEMP_OUT_L 0x1C
#define ITG3200_GYRO_XOUT_H 0x1D
#define ITG3200_GYRO_XOUT_L 0x1E
#define ITG3200_GYRO_YOUT_H 0x1F
#define ITG3200_GYRO_YOUT_L 0x20
#define ITG3200_GYRO_ZOUT_H 0x21
#define ITG3200_GYRO_ZOUT_L 0x22
#define ITG3200_PWR_MGM 0x3E

/* ------- Offset and scale factor calibration ------- */
#define X_OFFSET_AVERAGE 40.0f
#define Y_OFFSET_AVERAGE -3.0f
#define Z_OFFSET_AVERAGE -4.0f                        

#define CALIBRATE_X(X) (X - X_OFFSET_AVERAGE)
#define CALIBRATE_Y(Y) (Y - Y_OFFSET_AVERAGE)
#define CALIBRATE_Z(Z) (Z - Z_OFFSET_AVERAGE)

/* ------- Sensitivity gain ------- */
#define GAIN (1.0f / 14.375f) // ITG3200 sensitivity is 14.375 LSB/(degrees/sec)

/* ------- ITG3200 device address ------- */
#define ITG3200_DEVICE (0x68)

class ITG3200
{
	private:
		byte _buff[6] ; //6 bytes buffer for saving data read from the device
		void printByte(byte val); // print byte representation
		
	public:
		ITG3200();

		void writeTo(byte address, byte val); // write to register
		void readFrom(byte address, int num, byte buff[]); // read from register
		void readGyro(float* xyz); // read gyroscope
		void readDegPerSec(float *xyz); // read degrees/second
		void readRadPerSec(float *xyz); // read radians/second
		void printAllRegister();
};
#endif
