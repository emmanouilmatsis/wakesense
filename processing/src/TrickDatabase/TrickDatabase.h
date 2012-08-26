#ifndef TICKDATABASE_H
#define TICKDATABASE_H

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <sqlite3.h>
#include "Trick.h"
#include "../Converter/Converter.h"
#include "../TrickGenerator/TrickGenerator.h"

class TrickDatabase
{
	public:
		TrickDatabase(std::string filename);
		~TrickDatabase();
		Trick get(int rowid);
		Trick get(std::string name);
		void set(Trick trick);
		friend std::ostream& operator <<(std::ostream& out, TrickDatabase& object);

	private:
		sqlite3* database;

		void initializeDatabase();
		std::vector<std::vector<std::string> > queryDatabase(std::string statement);
		int getTrickRowid(std::string name, std::string type, std::string division, double difficulty, int yawMax, int pitchMax, int rollMax);
		int getLocationRowid(int trickRowid, int sampleRowid, int location);
		int getSampleRowid(int yaw, int pitch, int roll);
};

#endif // TRICKDATABASE_H
