#include "Arduino.h"
#include "ITG3200.h"
#include <Wire.h>

/* Private */

// Print byte
void ITG3200::printByte(byte val){
	int i;
	Serial.print("B");
	for(i=7; i>=0; i--){
		Serial.print(val >> i & 1, BIN);
	}
}

/* Public */

ITG3200::ITG3200() {
	// Join i2c bus (address optional for master)
	//Wire.begin();
}

// Writes val to address register on device
void ITG3200::writeTo(byte address, byte val) {
	Wire.beginTransmission(ITG3200_DEVICE); // start transmission to device 
	Wire.write(address); // send register address
	Wire.write(val); // send value to write
	Wire.endTransmission(); // end transmission
	delay(5); // delay 5ms after end of transmission
}

// Reads num bytes starting from address register on device in to _buff array
void ITG3200::readFrom(byte address, int num, byte _buff[]) {
	Wire.beginTransmission(ITG3200_DEVICE); // start transmission to device 
	Wire.write(address); // sends address to read from
	Wire.endTransmission(); // end transmission

	Wire.beginTransmission(ITG3200_DEVICE); // start transmission to device
	Wire.requestFrom(ITG3200_DEVICE, num); // request 6 bytes from device

	int i = 0;
	while(Wire.available())
	{ 
		_buff[i] = Wire.read(); // receive a byte
		i++;
	}

	Wire.endTransmission(); // end transmission
}

// Reads the measurments into one array of 3 elements x, y and z
void ITG3200::readGyro(float *xyz) {
	readFrom(ITG3200_GYRO_XOUT_H, 6, _buff); //read the acceleration data from the ITG3200

//	// Calibrated internal sensor axis representation data
//	xyz[0] = CALIBRATE_X((((int)(_buff[1]) << 8) | _buff[0])); // internal sensor x axis
//	xyz[1] = CALIBRATE_Y((((int)(_buff[3]) << 8) | _buff[2])); // internal sensor y axis
//	xyz[2] = CALIBRATE_Z((((int)(_buff[5]) << 8) | _buff[4])); // internal sensor z axis

	// Custom sensor axis representation data
	xyz[0] = CALIBRATE_Y(-1 * (((int)(_buff[2]) << 8) | _buff[3])); // internal sensor -y axis
	xyz[1] = CALIBRATE_X(-1 * (((int)(_buff[0]) << 8) | _buff[1])); // internal sensor -x axis
	xyz[2] = CALIBRATE_Z(-1 * (((int)(_buff[4]) << 8) | _buff[5])); // internal sensor -z axis  
}

// Translates the gyroscope readings to degrees/second
void ITG3200::readDegPerSec(float *xyz) {
	float xyz_float[3];
	readGyro(xyz_float);

	for(int i=0; i<3; i++) {
		xyz[i] = xyz_float[i] * GAIN;
	}
}

// Translates the gyroscope readings to radians/second
void ITG3200::readRadPerSec(float *xyz) {
	float xyz_float[3];
	readGyro(xyz_float);

	for(int i=0; i<3; i++) {
		xyz[i] = (xyz_float[i] * GAIN) * 0.01745329252;
	}
}

// Print all register value to the serial ouptut, which requires it to be setup
void ITG3200::printAllRegister() {
	byte _b;
	for (int i=21; i<=35; i++){
		Serial.print("0x");
		Serial.print(i, HEX);
		Serial.print(": ");
		readFrom(i, 1, &_b);
		printByte(_b);
		Serial.println("");    
	}
}
