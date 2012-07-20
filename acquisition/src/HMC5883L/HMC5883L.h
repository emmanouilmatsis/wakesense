#include "Arduino.h"

#ifndef HMC5883L_h
#define HMC5883L_h

/* ------- Register names ------- */
#define HMC5883L_CONFIG_A 0x00
#define HMC5883L_CONFIG_B 0x01
#define HMC5883L_MODE 0x02
#define HMC5883L_DATAX0 0x03
#define HMC5883L_DATAX1 0x04
#define HMC5883L_DATAY0 0x05
#define HMC5883L_DATAY1 0x06
#define HMC5883L_DATAZ0 0x07
#define HMC5883L_DATAZ1 0x08
#define HMC5883L_STATUS 0x09
#define HMC5883L_ID_A 0x0A
#define HMC5883L_ID_B 0x0B
#define HMC5883L_ID_C 0x0C

/* ------- Offset and scale factor calibration ------- */
#define X_MIN -575.0f
#define X_MAX 355.0f
#define Y_MIN -280.0f
#define Y_MAX 735.0f
#define Z_MIN -460.0f
#define Z_MAX 525.0f

#define X_OFFSET ((X_MIN + X_MAX) / 2.0f)
#define Y_OFFSET ((Y_MIN + Y_MAX) / 2.0f)
#define Z_OFFSET ((Z_MIN + Z_MAX) / 2.0f)
#define X_SCALE (100.0f / (X_MAX - X_OFFSET))
#define Y_SCALE (100.0f / (Y_MAX - Y_OFFSET))
#define Z_SCALE (100.0f / (Z_MAX - Z_OFFSET))

#define CALIBRATE_X(X) ((X - X_OFFSET) * X_SCALE)
#define CALIBRATE_Y(Y) ((Y - Y_OFFSET) * Y_SCALE)
#define CALIBRATE_Z(Z) ((Z - Z_OFFSET) * Z_SCALE)

/* ------- HMC5883L device address ------- */
#define HMC5883L_DEVICE (0x1E)

class HMC5883L
{
	private:
		byte _buff[6] ; //6 bytes buffer for saving data read from the device
		void printByte(byte val); // print byte representation
		
	public:
		HMC5883L();

		void writeTo(byte address, byte val); // write to register
		void readFrom(byte address, int num, byte buff[]); // read from register
		void readMagn(float* xyz); // read accelerometion
		void printAllRegister();
};
#endif
