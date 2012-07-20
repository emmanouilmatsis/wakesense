#include "Arduino.h"

#ifndef ADXL345_h
#define ADXL345_h

/* ------- Register names ------- */
#define ADXL345_DEVID 0x00
#define ADXL345_RESERVED1 0x01
#define ADXL345_THRESH_TAP 0x1d
#define ADXL345_OFSX 0x1e
#define ADXL345_OFSY 0x1f
#define ADXL345_OFSZ 0x20
#define ADXL345_DUR 0x21
#define ADXL345_LATENT 0x22
#define ADXL345_WINDOW 0x23
#define ADXL345_THRESH_ACT 0x24
#define ADXL345_THRESH_INACT 0x25
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27
#define ADXL345_THRESH_FF 0x28
#define ADXL345_TIME_FF 0x29
#define ADXL345_TAP_AXES 0x2a
#define ADXL345_ACT_TAP_STATUS 0x2b
#define ADXL345_BW_RATE 0x2c
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_INT_ENABLE 0x2e
#define ADXL345_INT_MAP 0x2f
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39

/* ------- Offset and scale factor calibration ------- */
#define X_MIN -250.0f
#define X_MAX 275.0f
#define Y_MIN -280.0f
#define Y_MAX 252.0f
#define Z_MIN -290.0f
#define Z_MAX 223.0f

#define X_OFFSET ((X_MIN + X_MAX) / 2.0f)
#define Y_OFFSET ((Y_MIN + Y_MAX) / 2.0f)
#define Z_OFFSET ((Z_MIN + Z_MAX) / 2.0f)
#define X_SCALE (256.0f / (X_MAX - X_OFFSET))
#define Y_SCALE (256.0f / (Y_MAX - Y_OFFSET))
#define Z_SCALE (256.0f / (Z_MAX - Z_OFFSET))

#define CALIBRATE_X(X) ((X - X_OFFSET) * X_SCALE)
#define CALIBRATE_Y(Y) ((Y - Y_OFFSET) * Y_SCALE)
#define CALIBRATE_Z(Z) ((Z - Z_OFFSET) * Z_SCALE)

/* ------- Gravity ------- */
#define G 0.0039f;

/* ------- ADXL345 device address ------- */
#define ADXL345_DEVICE (0x53)

class ADXL345
{
	private:
		byte _buff[6] ; //6 bytes buffer for saving data read from the device
		void printByte(byte val); // print byte representation
		
	public:
		ADXL345();

		void writeTo(byte address, byte val); // write to register
		void readFrom(byte address, int num, byte buff[]); // read from register
		void readAccel(float* xyz); // read accelerometion
    void readGxyz(float* xyzG); // read axis g force
    void readG(float* g); // read directionless g force
		void printAllRegister();
};
#endif
