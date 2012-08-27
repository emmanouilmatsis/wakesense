#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sqlite3.h>
#include "MLPTID/MLP.h"

using namespace std;

template <class T>
void printVector(const char* name, vector<T> input)
{
	cout << name << " = ";
	for (unsigned int i = 0; i < input.size(); i++)
		cout << input[i] << "\t";
	cout << endl;
}

void printTables()
{
	char* statement;
	char** result;
	int resultRow;
	int resultColumn;
	Database database;

	// Get nodes from samples
	const char* tables[] = {"hiddenLeft", "hidden", "hiddenRight"};

	for (unsigned int i = 0; i < 3; i++)
	{
		statement = sqlite3_mprintf("SELECT * FROM %q", tables[i]);

		database.query(statement, &result, &resultRow, &resultColumn);

		cout << endl;
		cout << "---------------- TABLE ----------------[" << tables[i] << endl;
		for (int i = 0; i < ((resultRow + 1) * resultColumn); i++)
		{
			cout << result[i] << "\t";
			if (!((i + 1) % resultColumn))
				cout << endl;
		}
		cout << "----------------------------------------" << endl;
		cout << endl;

		sqlite3_free(statement);
		database.queryFree(result);
	}
}

int main()
{
	MLP mlp;

	// wWorld, wRiver, wBank = 101, 102, 103
	// uWorldBank, uRiver, uEarth = 201, 202, 203

	// sampleIds
	int sampleIdsTemp[] = {101, 103};
	vector<int> sampleIds(sampleIdsTemp, sampleIdsTemp + (sizeof(sampleIdsTemp) / sizeof(int)));
	int sampleIdsTemp1[] = {102, 103};
	vector<int> sampleIds1(sampleIdsTemp1, sampleIdsTemp1 + (sizeof(sampleIdsTemp1) / sizeof(int)));
	int sampleIdsTemp2[] = {101};
	vector<int> sampleIds2(sampleIdsTemp2, sampleIdsTemp2 + (sizeof(sampleIdsTemp2) / sizeof(int)));
	// trickIds
	int trickIdsTemp[] = {201, 202, 203};
	vector<int> trickIds(trickIdsTemp, trickIdsTemp + (sizeof(trickIdsTemp) / sizeof(int)));

	int count = 30;
	while(count--)
	{
		mlp.trainNetwork(sampleIds, trickIds, 201);
		mlp.trainNetwork(sampleIds1, trickIds, 202);
		mlp.trainNetwork(sampleIds2, trickIds, 203);
	}

	printTables();
	printVector("result0", mlp.queryNetwork(sampleIds, trickIds));
	printVector("result1", mlp.queryNetwork(sampleIds1, trickIds));
	printVector("result2", mlp.queryNetwork(vector<int>(1, 103), trickIds));
	cout << endl;

	return 0;
}
