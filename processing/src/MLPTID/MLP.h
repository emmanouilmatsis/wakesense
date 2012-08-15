#ifndef MLP_H
#define MLP_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sqlite3.h>
#include "../Database/Database.h"

class MLP
{
	public:
		MLP();
		MLP(const char* filename);
		std :: vector<double> queryNetwork(std :: vector<int> sampleIds, std :: vector<int> trickIds);
		void trainNetwork(std :: vector<int> sampleIds, std :: vector<int> trickIds, int trickId);
		void print();

	private:
		Database database;
		std :: vector<int> sampleIds;
		std :: vector<int> hiddenIds;
		std :: vector<int> trickIds;
		std :: vector<double> ai;
		std :: vector<double> ah;
		std :: vector<double> ao;
		std :: vector<std :: vector<double> > wi;
		std :: vector<std :: vector<double> > wo;

		void setup(std :: vector<int> sampleIds, std :: vector<int> trickIds);
		std :: vector<double> feedForward();
		void backPropagate(std :: vector<double> targets, double N = 0.5);
		void updateDatabase();
		std :: vector<int> getAllHiddenIds(std :: vector<int> sampleIds, std :: vector<int> trickIds);
		void generateHidden(std :: vector<int> sampleIds, std :: vector<int> trickIds);
		double getStrength(const char* table, int from, int to);
		void setStrength(const char* table, int from, int to, double strength);
		double dtanh(double input);
		void createTables();
};

#endif // MLP_H
