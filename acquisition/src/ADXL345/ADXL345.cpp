#include "Arduino.h"
#include "ADXL345.h"
#include <Wire.h>

/* Private */

// Print byte
void ADXL345::printByte(byte val){
	int i;
	Serial.print("B");
	for(i=7; i>=0; i--){
		Serial.print(val >> i & 1, BIN);
	}
}

/* Public */

ADXL345::ADXL345() {
	// Join i2c bus (address optional for master)
	//Wire.begin();
}

// Writes val to address register on device
void ADXL345::writeTo(byte address, byte val) {
	Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device 
	Wire.write(address); // send register address
	Wire.write(val); // send value to write
	Wire.endTransmission(); // end transmission
  delay(5); // delay 5ms after end of transmission
}

// Reads num bytes starting from address register on device in to _buff array
void ADXL345::readFrom(byte address, int num, byte _buff[]) {
	Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device 
	Wire.write(address); // sends address to read from
	Wire.endTransmission(); // end transmission
  
	Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
	Wire.requestFrom(ADXL345_DEVICE, num); // request 6 bytes from device
  
	int i = 0;
	while(Wire.available())
	{ 
		_buff[i] = Wire.read(); // receive a byte
		i++;
	}
  
	Wire.endTransmission(); // end transmission
}

// Reads the acceleration into one array of 3 elements x, y and z
void ADXL345::readAccel(float *xyz) {
	readFrom(ADXL345_DATAX0, 6, _buff); //read the acceleration data from the ADXL345

  // Calibrated internal sensor axis representation data
  /*
   xyz[0] = CALIBRATE_X((((int)(_buff[1])  << 8) | _buff[0])); // internal sensor x axis
   xyz[1] = CALIBRATE_Y((((int)(_buff[3]) << 8) | _buff[2])); // internal sensor y axis
   xyz[2] = CALIBRATE_Z((((int)(_buff[5]) << 8) | _buff[4])); // internal sensor z axis
   */
  
  // Custom sensor axis representation data
  xyz[0] = CALIBRATE_Y((((int)(_buff[3]) << 8) | _buff[2])); // internal sensor y axis
  xyz[1] = CALIBRATE_X((((int)(_buff[1])  << 8) | _buff[0])); // internal sensor x axis
  xyz[2] = CALIBRATE_Z((((int)(_buff[5]) << 8) | _buff[4])); // internal sensor z axis  
}

// Translates the acceleration readings to individual axis G
void ADXL345::readGxyz(float *xyzG) {
	float xyz_float[3];
	readAccel(xyz_float);
  
	for(int i=0; i<3; i++){
		xyzG[i] = xyz_float[i] * G;
	}
}

// Translates the acceleration readings to total G
void ADXL345::readG(float *g) {
  float xyz[3];
  readAccel(xyz);
  
  *g = (sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2])) * G; // total G
}

// Print all register value to the serial ouptut, which requires it to be setup
void ADXL345::printAllRegister() {
	byte _b;
	Serial.print("0x00: ");
	readFrom(0x00, 1, &_b);
	printByte(_b);
	Serial.println("");
	int i;
	for (i=29;i<=57;i++){
		Serial.print("0x");
		Serial.print(i, HEX);
		Serial.print(": ");
		readFrom(i, 1, &_b);
		printByte(_b);
		Serial.println("");    
	}
}
