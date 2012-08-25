#ifndef TRICK_H
#define TRICK_H

#include <ostream>
#include <iomanip>
#include <string>
#include <vector>

struct Trick
{
	Trick();
  Trick(std::string name, std::string type, std::string division, double difficulty, int yawMax, int pitchMax, int rollMax, std::vector<int> yaw, std::vector<int> pitch, std::vector<int> roll);
	void print(std::ostream& out);

  std::string name;
	std::string type;
  std::string division;
	double difficulty;
	int yawMax;
	int pitchMax;
	int rollMax;
  std::vector<int> yaw;
  std::vector<int> pitch;
  std::vector<int> roll;
};

#endif // TRICK_H
