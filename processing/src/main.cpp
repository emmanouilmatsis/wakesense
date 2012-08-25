#include <iostream>
#include "Score/Score.h"

using namespace std;

int main()
{
	TrickDatabase tdb("trickDatabase.db");
	tdb.print(cout);
	Trick trick0 = tdb.get("BACKROLL");
	cout << endl;
	cout << trick0.name << endl;
	cout << trick0.type << endl;
	cout << trick0.division << endl;
	cout << trick0.difficulty << endl;
	cout << trick0.yawMax << endl;
	cout << trick0.pitchMax << endl;
	cout << trick0.rollMax << endl;
	cout << trick0.yaw[0] << endl;
	cout << trick0.pitch[0] << endl;
	cout << trick0.roll[0] << endl;
	cout << endl;
	Trick trick1 = tdb.get("MOBIUS");
	cout << endl;
	cout << trick1.name << endl;
	cout << trick1.type << endl;
	cout << trick1.division << endl;
	cout << trick1.difficulty << endl;
	cout << trick1.yawMax << endl;
	cout << trick1.pitchMax << endl;
	cout << trick1.rollMax << endl;
	cout << trick1.yaw[0] << endl;
	cout << trick1.pitch[0] << endl;
	cout << trick1.roll[0] << endl;
	cout << endl;

	/*
	int dataTemp0[] = {0, 0, 0};
	vector<int> data0(dataTemp0, dataTemp0 + sizeof(dataTemp0) / sizeof(int));
	int dataTemp1[] = {40, 60, 30};
	vector<int> data1(dataTemp1, dataTemp1 + sizeof(dataTemp1) / sizeof(int));
	int dataTemp2[] = {50, 70, 30};
	vector<int> data2(dataTemp2, dataTemp2 + sizeof(dataTemp2) / sizeof(int));
	int dataTemp3[] = {60, 70, 30};
	vector<int> data3(dataTemp3, dataTemp3 + sizeof(dataTemp3) / sizeof(int));
	int dataTemp4[] = {0, 0, 0};
	vector<int> data4(dataTemp4, dataTemp4 + sizeof(dataTemp4) / sizeof(int));

	std::vector<std::vector<int> > data;
	data.push_back(data0);
	data.push_back(data1);
	data.push_back(data2);
	data.push_back(data3);
	data.push_back(data4);

	Score score;

	cout << endl;
	score.run("FRONTSIDE_360", 1, data);
	cout << "Execution: " << score.getExecution() << endl;
  cout << "Intensity: " << score.getIntensity() << endl;
	cout << "Composition: " << score.getComposition() << endl;
	cout << "Total: " << score.getTotal() << endl;
	cout << endl;

	cout << endl;
	score.run("BACKROLL", 1, data);
	cout << "Execution: " << score.getExecution() << endl;
  cout << "Intensity: " << score.getIntensity() << endl;
	cout << "Composition: " << score.getComposition() << endl;
	cout << "Total: " << score.getTotal() << endl;
	cout << endl;

	cout << endl;
	score.run("BACKROLL_TO_BLIND", 1, data);
	cout << "Execution: " << score.getExecution() << endl;
  cout << "Intensity: " << score.getIntensity() << endl;
	cout << "Composition: " << score.getComposition() << endl;
	cout << "Total: " << score.getTotal() << endl;
	cout << endl;
	*/

	return 0;
}
