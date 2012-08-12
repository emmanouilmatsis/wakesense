#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "MLPTID/MLP.h"

using namespace std;

/* ---------------- */

void print(vector<double> input)
{
	cout << endl;
	for (unsigned int i = 0; i < input.size(); i++)
		cout << input[i] << endl;
}

int main()
{
	MLP mlp;

	// wWorld, wRiver, wBank = 101, 102, 103
	// uWorldBank, uRiver, uEarth = 201, 202, 203

	// Train
	int sampleIdsTemp[] = {101, 103};
	vector<int> sampleIds(sampleIdsTemp, sampleIdsTemp + (sizeof(sampleIdsTemp) / sizeof(int)));

	int trickIdsTemp[] = {201, 202, 203};
	vector<int> trickIds(trickIdsTemp, trickIdsTemp + (sizeof(trickIdsTemp) / sizeof(int)));


	/* ================ */
	/* generateHidden   */
	/* ================ */


	{
	mlp.generateHidden(sampleIds, trickIds);

	char statement[128];
	char** result;
	int resultRow;
	int resultColumn;
	Database database;


	strcpy(statement, "SELECT * FROM hiddenLeft");
	cout << endl << statement << endl;
	database.query(statement, &result, &resultRow, &resultColumn);

	for (int i = 0; i < ((resultRow + 1) * resultColumn); i++)
	{
		cout << result[i] << "\t";
		if (!((i + 1) % resultColumn))
			cout << endl;
	}
	cout << endl;
	database.queryFree(result);


	strcpy(statement, "SELECT * FROM hiddenRight");
	cout << endl << statement << endl;
	database.query(statement, &result, &resultRow, &resultColumn);

	for (int i = 0; i < ((resultRow + 1) * resultColumn); i++)
	{
		cout << result[i] << "\t";
		if (!((i + 1) % resultColumn))
			cout << endl;
	}
	cout << endl;
	database.queryFree(result);
	}


	/* ================ */
	/* getStrength			*/
	/* ================ */


	{
	char statement[128];
	char** result;
	int resultRow;
	int resultColumn;
	Database database;
	vector<int> hiddenIds;


	strcpy(statement, "SELECT rowid FROM hidden");
	database.query(statement, &result, &resultRow, &resultColumn);

  for (int i = 0; i < resultRow; i++)
	{
		hiddenIds.push_back(atoi(result[i + resultColumn]));
		cout << "hiddenIds[" << i << "] = " << hiddenIds[i] << endl;
	}
	cout << endl;

	database.queryFree(result);

	cout << "hiddenLeft" << endl;
	cout << "sample\t\ttrickId\t\tstrength" << endl;
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
		{
			cout << sampleIds[i] << "\t\t" << hiddenIds[j] << "\t\t" << mlp.getStrength("hiddenLeft", sampleIds[i], hiddenIds[j]) << endl;
		}
	cout << endl;

	cout << "hiddenRight" << endl;
	cout << "sampleId\ttrickId\t\tstrength" << endl;
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
		for (unsigned int j = 0; j < trickIds.size(); j++)
		{
			cout << hiddenIds[i] << "\t\t" << trickIds[j] << "\t\t" << mlp.getStrength("hiddenRight", hiddenIds[i], trickIds[j]) << endl;
		}
	cout << endl;
	}


	/* ================ */
	/* setStrength      */
	/* ================ */

	return 0;
}
