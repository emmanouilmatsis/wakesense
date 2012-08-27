#ifndef SUPERVISEDTRICKIDENTIFIERTID_H
#define SUPERVISEDTRICKIDENTIFIERTID_H

#include <cstdlib>
#include <ostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <sqlite3.h>
#include "../Database/Database.h"

class SupervisedTrickIdentifier
{
	public:
		SupervisedTrickIdentifier();
		SupervisedTrickIdentifier(const char* filename);
		std::vector<double> queryNetwork(std::vector<int> sampleIds, std::vector<int> trickIds);
		void trainNetwork(std::vector<int> sampleIds, std::vector<int> trickIds, int trickId);
		int getId();
		double getCorrelation();
		friend std::ostream& operator <<(std::ostream& out, const SupervisedTrickIdentifier& object);

	private:
		Database database;
		std::vector<int> sampleIds;
		std::vector<int> hiddenIds;
		std::vector<int> trickIds;
		std::vector<double> ai;
		std::vector<double> ah;
		std::vector<double> ao;
		std::vector<std::vector<double> > wi;
		std::vector<std::vector<double> > wo;
		std::vector<double> result;

		void setup(std::vector<int> sampleIds, std::vector<int> trickIds);
		std::vector<double> feedForward();
		void backPropagate(std::vector<double> targets, double N = 0.5);
		void updateDatabase();
		std::vector<int> getAllHiddenIds(std::vector<int> sampleIds, std::vector<int> trickIds);
		void generateHidden(std::vector<int> sampleIds, std::vector<int> trickIds);
		double getStrength(const char* table, int from, int to);
		void setStrength(const char* table, int from, int to, double strength);
		double dtanh(double input);
		void createTables();
};

#endif // SUPERVISEDTRICKIDENTIFIERTID_H
