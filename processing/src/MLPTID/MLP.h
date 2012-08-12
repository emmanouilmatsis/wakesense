#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sqlite3.h>
#include "../Database/Database.h"

#include <iostream>
using namespace std;

class MLP
{
	public:
		MLP();
		MLP(const char* filename);
		vector<double> queryNetwork(vector<int> sampleIds, vector<int> trickIds);
		void trainNetwork(vector<int> sampleIds, vector<int> trickIds, int trickId);

		void setup(vector<int> sampleIds, vector<int> trickIds);
		vector<double> feedForward();
		void backPropagate(vector<double> targets, double N = 0.5);
		void update();
		vector<int> getAllHiddenIds(vector<int> sampleIds, vector<int> trickIds);
		void generateHidden(vector<int> sampleIds, vector<int> trickIds);
		double getStrength(const char* table, int from, int to);
		void setStrength(const char* table, int from, int to, double strength);
		double dtanh(double input);
		void createTables();

	private:
		Database database;
		vector<int> sampleIds;
		vector<int> hiddenIds;
		vector<int> trickIds;
		vector<double> ai;
		vector<double> ah;
		vector<double> ao;
		vector<vector<double> > wi;
		vector<vector<double> > wo;

		void print(vector<double> input);
		void print(vector<int> input);
};
