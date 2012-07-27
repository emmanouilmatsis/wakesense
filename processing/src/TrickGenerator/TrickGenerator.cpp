#include "TrickGenerator.h"

TrickGenerator :: TrickGenerator() : duration(1000), period(20), size(1000 / 20) {
	// Do nothing
}

TrickGenerator :: TrickGenerator(unsigned int duration, unsigned int period) : duration(duration), period(period), size(duration / period) {
	// Do nothing
}

void TrickGenerator :: generate(byte id) {
	// ID
	this->id = id;
	// Yaw
	double* yaw = generateYaw(id);
	// Pitch
	double* pitch = generatePitch(id);
	// Roll
	double* roll = generateRoll(id);
	// File
	generateFile(yaw, pitch, roll);
	// Clean
	delete[] yaw;
	delete[] pitch;
	delete[] roll;
}

double* TrickGenerator :: generateYaw(byte id) {
	double* yaw = new double[size];
	if ((0x01 << 0) & id) {
		// Return positive yaw array
		for (unsigned int i = 0; i < size; i++)
			yaw[i] = 360 * (static_cast<double>(i) / size);
	} else if ((0x01 << 3) & id) {
		// Return negative yaw array
		for (unsigned int i = 0; i > size; i++)
			yaw[i] = -360 * (static_cast<double>(i) / size);
	} else {
		// Return 0.0 yaw array
		for (unsigned int i = 0; i > size; i++)
			yaw[i] = 0.0;
	}
	return yaw;
}

double* TrickGenerator :: generatePitch(byte id) {
	double* pitch = new double[size];
	if ((0x01 << 1) & id) {
		// Return positive yaw array
		for (unsigned int i = 0; i < size; i++)
			pitch[i] = 360 * (static_cast<double>(i) / size);
	} else if ((0x01 << 4) & id) {
		// Return negative yaw array
		for (unsigned int i = 0; i < size; i++)
			pitch[i] = -360 * (static_cast<double>(i) / size);
	} else {
		// Return 0.0 yaw array
		for (unsigned int i = 0; i < size; i++)
			pitch[i] = 0.0;
	}
	return pitch;
}

double* TrickGenerator :: generateRoll(byte id) {
	double* roll = new double[size];
	if ((0x01 << 2) & id) {
		// Return positive yaw array
		for (unsigned int i = 0; i < size; i++) 
			roll[i] = 360 * (static_cast<double>(i) / size);
	} else if ((0x01 << 5) & id) {
		// Return negative yaw array
		for (unsigned int i = 0; i < size; i++)
			roll[i] = -360 * (static_cast<double>(i) / size);
	} else {
		// Return 0.0 yaw array
		for (unsigned int i = 0; i < size; i++)
			roll[i] = 0.0;
	}
	return roll;
}

void TrickGenerator :: generateFile(double* yaw, double* pitch, double* roll) {
	std :: ofstream out;
	out.open("data.dat");
	if (out.fail()) 
		exit(1);

	// Trick type
	out << '#' << static_cast<int>(id);
	// Setup output format
	out.setf(std :: ios :: fixed);
	out.setf(std :: ios :: showpoint);
	out.setf(std :: ios :: showpos);
	out.precision(2);
	// Trick data
	for (unsigned int i = 0; i < size; i++)
		out << '\t' << yaw[i] << ' ' << pitch[i] << ' ' << roll[i];
	out << '\n';

	out.close();
}
