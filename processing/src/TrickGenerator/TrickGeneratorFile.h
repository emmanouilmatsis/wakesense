#ifndef TRICKGENERATORFILE_H
#define TRICKGENERATORFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include "../TrickDatabase/TrickDatabase.h"

#define DURATION 1000
#define INTERVAL 20

class TrickGeneratorFile
{
	public:
		TrickGeneratorFile(std::string filename);
		void generate();

	private:
		TrickDatabase trickDatabase;
};

#endif // TRICKGENERATORFILE_H
