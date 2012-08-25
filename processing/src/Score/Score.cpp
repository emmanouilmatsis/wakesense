#include "Score.h"

/* -------- Public -------- */

Score :: Score() : trickDatabase("TrickDatabase.db"), composition(0), total(0)
{
}

/* ---------------- */

void Score :: run(std::string name, double correlation, std::vector<std::vector<int> > data)
{
  Trick trick = trickDatabase.get(name);

  // Execution
  int end[3] = {trick.yawMax, trick.pitchMax, trick.rollMax}; // expected rotation {yaw, pitch, roll}
  int remaining[3] = {0, 0, 0}; // remaining rotation {yaw, pitch, roll}

  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < data.size(); j++)
    {
      if (data[j][i] > 0)
        remaining[i] = (((end[i] % 360) == 0) ? 0 : 180) - data[j][i];
      else
        remaining[i] = (((end[i] % 360) == 0) ? 0 : -180) - data[j][i];
    }

  execution = (((((180 - abs(remaining[0])) / 180.0) + ((180 - abs(remaining[1])) / 180.0) + ((180 - abs(remaining[2])) / 180.0)) / 3) + correlation) / 2;

  // Intensity
  intensity = trick.difficulty * execution;

  // Composition

	// Total
  if (total == 0)
    total = trick.difficulty * execution;
  else
    total = (total + trick.difficulty * execution) / 2;

  //TODO
  std::cout
      << "Name: " << trick.name << std::endl
			<< "Division: " << trick.division << std::endl
			<< "Difficulty: " << trick.difficulty << std::endl
			<< "YawMax, PitchMax, RollMax: " << trick.yawMax << ", " << trick.pitchMax << ", " << trick.rollMax 
			<< std::endl;

}

/* ---------------- */

double Score :: getExecution()
{
  return execution;
}

/* ---------------- */

double Score :: getIntensity()
{
  return intensity;
}

/* ---------------- */

double Score :: getComposition()
{
  return composition;
}

/* ---------------- */

double Score :: getTotal()
{
	return total;
}

