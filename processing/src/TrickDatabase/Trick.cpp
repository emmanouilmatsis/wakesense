#include "Trick.h"

/* -------- Public -------- */

Trick :: Trick()
		: name("NULL"), type("NULL"), division("NULL"), difficulty(0), yawMax(0), pitchMax(0), rollMax(0)
{
}

/* ---------------- */

Trick :: Trick(std::string name, std::string type, std::string division, double difficulty, int yawMax, int pitchMax, int rollMax, std::vector<int> yaw, std::vector<int> pitch, std::vector<int> roll)
    : name(name), type(type), division(division), difficulty(difficulty), yawMax(yawMax), pitchMax(pitchMax), rollMax(rollMax), yaw(yaw), pitch(pitch), roll(roll)
{
}

/* ---------------- */

void Trick :: print(std::ostream& out)
{

  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : Trick" << std::endl
      << "field : Name, Type, Division, Difficulty," << std::endl
      << "        YawMax, PitchMax, RollMax," << std::endl
      << "        Yaw, Pitch, Roll" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl
      << "Name : " << name << std::endl
      << "Type : " << type << std::endl
      << "Division : " << division << std::endl
      << "Difficulty : " << difficulty << std::endl
      << "YawMax : " << yawMax << std::endl
      << "PitchMax : " << pitchMax << std::endl
      << "RollMax : " << rollMax << std::endl
      << std::endl
      << std::setw(10) << std::left << "Yaw"
      << std::setw(10) << std::left << "Pitch"
      << std::setw(10) << std::left << "Roll"
      << std::endl;
  out.setf(std::ios::showpos);
  for (unsigned int i = 0; i < yaw.size(); i++)
  {
    out
        << std::setw(10) << std::left << yaw[i]
        << std::setw(10) << std::left << pitch[i]
        << std::setw(10) << std::left << roll[i]
        << std::endl;
  }
}

