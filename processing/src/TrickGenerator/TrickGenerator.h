#include <iostream>
#include <fstream>
#include <cstdlib>

/* -------- Trick List -------- */
#define FRONTSIDE_360 0x01 // yaw = 360, pitch = 0, roll = 0
#define BACKROLL 0x02 // yaw = 0, pitch = 360, roll = 0
#define TANTRUM 0x04 // yaw = 0, pitch = 0, roll = 360
#define MOBE 0x06 // yaw = 360, pitch = 360, roll = 0
#define MOBY_DICK 0x05 // yaw = -360, pitch = 0, roll = 360


/* -------- Type Definition -------- */
typedef unsigned char byte;

class TrickGenerator {
	public:
	TrickGenerator();
	TrickGenerator(unsigned int duration, unsigned int period);
	void generate(byte id);

	private:
	byte id;
  unsigned int duration;
	unsigned int period;
	unsigned int size;
	double* generateYaw(byte id);
	double* generatePitch(byte id);
	double* generateRoll(byte id);
	void generateFile(double* yaw, double* pitch, double* roll);
};
