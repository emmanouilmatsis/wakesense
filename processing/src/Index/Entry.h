#ifndef ENTRY_H
#define ENTRY_H

#include <iostream>
#include <cstring>
#include <vector>

struct Entry
{
	char* name;
	int trickId;
	int version;
	int versionId;
	unsigned int size;
	std :: vector<int> yaw;
	std :: vector<int> pitch;
	std :: vector<int> roll;
	std :: vector<int> sampleIds;

	Entry();
	Entry(const char* name, std :: vector<int> yaw, std :: vector<int> pitch, std :: vector<int> roll);
	Entry(const Entry& structure);
	~Entry();
	void operator =(const Entry& structure);
	void print();
};

#endif // ENTRY_H
