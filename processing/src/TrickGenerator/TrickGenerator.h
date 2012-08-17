#ifndef TRICKGENERATOR_H
#define TRICKGENERATOR_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

/* -------- Trick List -------- */

#define FRONTSIDE_360 0x01 // yaw = 360, pitch = 0, roll = 0
#define BACKROLL 0x02 // yaw = 0, pitch = 360, roll = 0
#define TANTRUM 0x04 // yaw = 0, pitch = 0, roll = 360
#define MOBE 0x06 // yaw = 360, pitch = 360, roll = 0
#define MOBY_DICK 0x05 // yaw = -360, pitch = 0, roll = 360

/* -------- Constant -------- */

#define PI 3.14159265


/* -------- Type Definition -------- */

typedef unsigned char byte;

/* -------- Class Definition -------- */

class TrickGenerator
{
	public:
		TrickGenerator();
		TrickGenerator(unsigned int duration, unsigned int period);
		void generate(byte id);

	private:
		byte id;
		int* yaw;
		int* pitch;
		int* roll;
		unsigned int duration;
		unsigned int period;
		unsigned int size;
		int* generateYaw();
		int* generatePitch();
		int* generateRoll();
		void generateFile();
		const char* generateName();
};

#endif // TRICKGENERATOR_H 
