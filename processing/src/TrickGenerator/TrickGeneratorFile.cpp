#include "TrickGeneratorFile.h"

TrickGeneratorFile :: TrickGeneratorFile(std::string filename)
	: trickDatabase(filename)
{
}

void TrickGeneratorFile :: generate()
{
  std :: ofstream out;

  std::string filename;
	std::cout << "Enter filename: ";
	std::cin >> filename;

	std::string name;
	std::cout << "Enter trick name: ";
	std::cin >> name;

	Trick trick = trickDatabase.get(name);
	
  out.open(filename.c_str(), std::ios::app);
  if (out.fail())
    exit(1);

  // Trick type
  out << name;
  // Trick data
  for (unsigned int i = 0; i < DURATION / INTERVAL; i++)
    out << '\t' << trick.yaw[i] << ' ' << trick.pitch[i] << ' ' << trick.roll[i];
  out << '\n';

  out.close();
}

