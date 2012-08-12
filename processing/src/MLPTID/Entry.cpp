#include "Entry.h"

Entry :: Entry() {
	name = new char[32];
  size = 100;
	yaw = new int[size];
	pitch = new int[size];
	roll = new int[size];
}

Entry :: Entry(const char* name, int size, const int* yaw, const int* pitch, const int* roll) {
	this->name = new char[32];
  this->size = size;
	this->yaw = new int[size];
	this->pitch = new int[size];
	this->roll = new int[size];
	
	strcpy(this->name, name);
	for (int i = 0; i < size; i++) {
		this->yaw[i] = yaw[i];
		this->pitch[i] = pitch[i];
		this->roll[i] = roll[i];
	}
}

Entry :: Entry(const Entry& structure) {
	this->name = new char[32];
  this->size = structure.size;
	this->yaw = new int[structure.size];
	this->pitch = new int[structure.size];
	this->roll = new int[structure.size];
	
	strcpy(this->name, structure.name);
	for (int i = 0; i < structure.size; i++) {
		this->yaw[i] = structure.yaw[i];
		this->pitch[i] = structure.pitch[i];
		this->roll[i] = structure.roll[i];
	}
}

Entry :: ~Entry() {
	delete[] name;
	delete[] yaw;
	delete[] pitch;
	delete[] roll;
}

void Entry :: operator =(const Entry& structure) {
	delete[] name;
	delete[] yaw;
	delete[] pitch;
	delete[] roll;

	this->name = new char[32];
  this->size = structure.size;
	this->yaw = new int[structure.size];
	this->pitch = new int[structure.size];
	this->roll = new int[structure.size];
	
	strcpy(this->name, structure.name);
	for (int i = 0; i < structure.size; i++) {
		this->yaw[i] = structure.yaw[i];
		this->pitch[i] = structure.pitch[i];
		this->roll[i] = structure.roll[i];
	}
}
