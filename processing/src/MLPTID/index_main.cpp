#include <iostream>
#include "Parser/Parser.h"
#include "MLPTID/Index.h"

using namespace std;

int main()
{   
	Parser parser("../data/base.dat");

	double*** data = parser.getData();
	vector<int> yawTemp(parser.getColumnSize(), 0);
	vector<int> pitchTemp(parser.getColumnSize(), 0);
	vector<int> rollTemp(parser.getColumnSize(), 0);

for (unsigned int j = 0; j < parser.getRowSize(); j++)
{
	for (unsigned int i = 0; i < parser.getColumnSize(); i++)
	{
		yawTemp[i] = static_cast<int>(data[j][i][0]);
		pitchTemp[i] = static_cast<int>(data[j][i][1]);
		rollTemp[i] = static_cast<int>(data[j][i][2]);
	}

	Entry entry ("FRONTSIDE_360", yawTemp, pitchTemp, rollTemp);

	Index index;
	index.setEntry(entry);
	entry.print();
	index.print();

	vector<int> trickIds = index.getEntryIds();
	cout << "trickIds = ";
	for (unsigned int i = 0; i < trickIds.size(); i++)
		cout << trickIds[i] << "\t";
	cout << endl;

	index.getEntry("FRONTSIDE_360", 6).print();
	}

	return 0;
}
