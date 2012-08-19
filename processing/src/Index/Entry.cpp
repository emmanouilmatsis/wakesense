#include "Entry.h"

Entry :: Entry()
{
  name = new char[32];
  this->trickId = -1;
  this->version = -1;
  this->versionId = -1;
  size = 0;
}

Entry :: Entry(const char* name, std::vector<int> yaw, std::vector<int> pitch, std::vector<int> roll)
{
  this->name = new char[32];

  strcpy(this->name, name);
  this->trickId = -1;
  this->version = -1;
  this->versionId = -1;
  this->size = yaw.size();
  this->yaw = yaw;
  this->pitch = pitch;
  this->roll = roll;
  this->sampleIds = std::vector<int>(yaw.size(), -1);
}

Entry :: Entry(const Entry& structure)
{
  this->name = new char[32];

  strcpy(this->name, structure.name);
  this->trickId = structure.trickId;
  this->version = structure.version;
  this->versionId = structure.versionId;
  this->size = structure.yaw.size();
  this->yaw = structure.yaw;
  this->pitch = structure.pitch;
  this->roll = structure.roll;
  this->sampleIds = structure.sampleIds;
}

Entry :: ~Entry()
{
  delete[] name;
}

void Entry :: operator =(const Entry& structure)
{
  delete[] name;

  this->name = new char[32];

  strcpy(this->name, structure.name);
  this->trickId = structure.trickId;
  this->version = structure.version;
  this->versionId = structure.versionId;
  this->size = structure.yaw.size();
  this->yaw = structure.yaw;
  this->pitch = structure.pitch;
  this->roll = structure.roll;
  this->sampleIds = structure.sampleIds;
}

void Entry :: print(std::ostream& out)
{

  out.setf(std::ios::showpos);
  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : Entry" << std::endl
      << "field : Name, TrickId, Version, VersionId, Size," << std::endl
      << "        Yaw, Pitch, Roll, SampleIds" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl
      << "Name : " << name << std::endl
      << "TrickId : " << trickId << std::endl
      << "Version : " << version << std::endl
      << "VersionId : " << versionId << std::endl
      << "Size : " << size << std::endl
      << std::endl
      << std::setw(10) << std::left << "Yaw"
      << std::setw(10) << std::left << "Pitch"
      << std::setw(10) << std::left << "Roll"
      << std::setw(10) << std::left << "SampleIds"
      << std::endl;
  for (unsigned int i = 0; i < size; i++)
  {
    out
        << std::setw(10) << std::left << yaw[i]
        << std::setw(10) << std::left << pitch[i]
        << std::setw(10) << std::left << roll[i]
        << std::setw(10) << std::left << sampleIds[i]
        << std::endl;
  }
}
