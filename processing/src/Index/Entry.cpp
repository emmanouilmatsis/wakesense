#include "Entry.h"

Entry :: Entry()
{
	name = new char[32];
	this->trickId = -1;
	this->version = -1;
	this->versionId = -1;
	size = 0;
}

Entry :: Entry(const char* name, std :: vector<int> yaw, std :: vector<int> pitch, std :: vector<int> roll)
{
	this->name = new char[32];

	strcpy(this->name, name);
	this->trickId = -1;
	this->version = -1;
	this->versionId = -1;
	this->size = yaw.size();
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
	this->sampleIds = std :: vector<int>(yaw.size(), -1);
}

Entry :: Entry(const Entry& structure)
{
	this->name = new char[32];

	strcpy(this->name, structure.name);
	this->trickId = structure.trickId;
	this->version = structure.version;
	this->versionId = structure.versionId;
	this->size = structure.yaw.size();
	this->yaw = structure.yaw;
	this->pitch = structure.pitch;
	this->roll = structure.roll;
	this->sampleIds = structure.sampleIds;
}

Entry :: ~Entry()
{
	delete[] name;
}

void Entry :: operator =(const Entry& structure)
{
	delete[] name;

	this->name = new char[32];

	strcpy(this->name, structure.name);
	this->trickId = structure.trickId;
	this->version = structure.version;
	this->versionId = structure.versionId;
	this->size = structure.yaw.size();
	this->yaw = structure.yaw;
	this->pitch = structure.pitch;
	this->roll = structure.roll;
	this->sampleIds = structure.sampleIds;
}

void Entry :: print()
{
	std :: cout << "---------------- Entry ----------------" << std :: endl;
	std :: cout << "entry.name = " << name << std :: endl;
	if (trickId != -1) std :: cout << "entry.trickId = " << trickId << std :: endl;
	if (version != -1) std :: cout << "entry.version = " << version << std :: endl;
	if (versionId != -1) std :: cout << "entry.versionId = " << versionId << std :: endl;
	std :: cout << "entry.size = " << size << std :: endl;
	for (unsigned int i = 0; i < size; i++)
	{
		std :: cout << "entry.yaw[" << i << "] = " << yaw[i] << "\t";
		std :: cout << "entry.pitch[" << i << "] = " << pitch[i] << "\t";
		std :: cout << "entry.roll[" << i << "] = " << roll[i] << "\t";
		if (sampleIds[i] != -1) std :: cout << "entry.sampleIds[" << i << "] = " << sampleIds[i] << "\t";
		std :: cout << std :: endl;
	}
	std :: cout << "---------------------------------------" << std :: endl;
}
