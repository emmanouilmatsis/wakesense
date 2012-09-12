#include "Score.h"

/* -------- Public -------- */

Score :: Score(std::string filename)
  : trickDatabase(filename), execution(0), intensity(0), composition(0), total(0)
{
}

/* ---------------- */

void Score :: run(std::string name, double correlation, std::vector<std::vector<int> > data)
{
  // Trick from database
  trick.push_back(trickDatabase.get(name));

	// Division
	runDivision();

	// Execution
	runExecution(correlation, data);

	// Intensity
	runIntensity();

	// Composition
	runComposition();

  // Total
	runTotal();
}

/* ---------------- */

std::ostream& operator <<(std::ostream& out, const Score& object)
{
  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : Score" << std::endl
      << "field : Name, Type, Division, Difficulty, Rotation," << std::endl
      << "      : Division Score, Execution Score," << std::endl
      << "      : Intensity Score, Composition Score, Total Score" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl;

	out.precision(2);

  out
      << std::setw(20) << std::left << "Name"
      << std::setw(20) << std::left << "Type"
      << std::setw(20) << std::left << "Division"
      << std::setw(20) << std::left << "Difficulty"
      << std::setw(20) << std::left << "Rotation"
      << std::setw(20) << std::left << "Division Score"
      << std::setw(20) << std::left << "Execution Score"
      << std::setw(20) << std::left << "Intensity Score"
      << std::setw(20) << std::left << "Composition Score"
      << std::setw(20) << std::left << "Total Score"
      << std::endl
      << std::setw(20) << std::left << object.trick.back().name
      << std::setw(20) << std::left << object.trick.back().type
      << std::setw(20) << std::left << object.trick.back().division
      << std::setw(20) << std::left << object.trick.back().difficulty
      << std::setw(5) << std::left << object.trick.back().yawMax 
      << std::setw(5) << std::left << object.trick.back().pitchMax 
      << std::setw(10) << std::left << object.trick.back().rollMax;

	out.unsetf(std::ios::showpos);
	out	
      << std::setw(20) << std::left << object.division
      << std::setw(5) << std::left << object.execution * 100 << std::setw(15) << std::left << "%"
      << std::setw(5) << std::left << object.intensity * 100 << std::setw(15) << std::left << "%"
      << std::setw(5) << std::left << object.composition * 100 << std::setw(15) << std::left << "%"
      << std::setw(5) << std::left << object.total * 100 << std::setw(15) << std::left << "%"
      << std::endl
      << std::endl;

  return out;
}

/* -------- Private -------- */

void Score :: runDivision()
{
	std::map<std::string, int> divisionTable;
	divisionTable["NOVICE"] = 0;
	divisionTable["INTERMEDIATE"] = 1;
	divisionTable["ADVANCED"] = 2;
	divisionTable["EXPERT"] = 3;
	divisionTable["OUTLAW"] = 4;

	int value = divisionTable[trick[0].division];
	division = trick[0].division;

	std::vector<Trick>::iterator iter;
	for (iter = trick.begin(); iter != trick.end(); iter++)
		if (divisionTable[iter->division] > value)
		{
			value = divisionTable[iter->division];
			division = iter->division;
		}

}

/* ---------------- */

void Score :: runExecution(double correlation, std::vector<std::vector<int> > data)
{
	// Rotation completion
	double completion;
  int end[3] = {trick.back().yawMax, trick.back().pitchMax, trick.back().rollMax};
  int remaining[3] = {0, 0, 0};

  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < data.size(); j++)
      if (data[j][i] > 0)
        remaining[i] = (((end[i] % 360) == 0) ? 0 : 180) - data[j][i];
      else
        remaining[i] = (((end[i] % 360) == 0) ? 0 : -180) - data[j][i];

	completion = (((180 - abs(remaining[0])) / 180.0) + ((180 - abs(remaining[1])) / 180.0) + ((180 - abs(remaining[2])) / 180.0)) / 3;

	// Execution
  execution = (completion + correlation) / 2;
}

/* ---------------- */

void Score :: runIntensity()
{
	// Intensity
  intensity = trick.back().difficulty * execution;
}

/* ---------------- */

void Score :: runComposition()
{
  // Gini impurity
	double impurity = 0.0;
  std::map<std::string, int> type;

	std::vector<Trick>::iterator iter0;
	for (iter0 = trick.begin(); iter0 != trick.end(); iter0++)
    type[iter0->type]++;

	std::map<std::string, int>::iterator iter1;
	std::map<std::string, int>::iterator iter2;
	for (iter1 = type.begin(); iter1 != type.end(); iter1++)
		for (iter2 = type.begin(); iter2 != type.end(); iter2++)
		{
			if (iter1->first == iter2->first) continue;
			double d1 = iter1->second / static_cast<double>(trick.size());
			double d2 = iter2->second / static_cast<double>(trick.size());
			impurity += d1 * d2;
		}

	// Composition
	composition = impurity;
}

/* ---------------- */

void Score :: runTotal()
{
  if (total == 0)
    total = (execution + intensity + composition) / 3;
  else
    total = (total + (execution + intensity + composition) / 3) / 2;
}
