#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Parser/Parser.h"
#include "Receiver/Receiver.h"
#include "Index/Index.h"
#include "PCCTID/PCCTID.h"
#include "MLPTID/MLPTID.h"

/* -------- Constant -------- */

#define MIN_ARGC 2

/* -------- Using -------- */

using namespace std;

/* -------- Main -------- */

int main(int argc, char** argv)
{
  // Argument variables
  std::string type;
  bool in = false;
  bool out = false;
  bool print = false;

  // Parse command line arguments
  if (argc < MIN_ARGC)
  {
    std::cout << "Usage: [-t <PCC2D | PCC3D | MLPQuery | MLPTrain>] [-i] [-o] [-p] " << std::endl;
    exit(0);
  }
  else
  {
    for (int i = 1; i < argc; i++)
    {
      std::string arg(argv[i]);

      if (arg == "-t")
      {
        type = argv[++i];
      }
      else if (arg == "-i")
      {
        in = true;
      }
      else if (arg == "-o")
      {
        out = true;
      }
      else if (arg == "-p")
      {
        print = true;
      }
      else
      {
        std::cout << "Invalid arguments, please try again." << std::endl;
        exit(0);
      }
    }
  }

// Run
  std::vector<std::vector<int> > data;

  while (true)
  {
    // Get data
    if (in) // from file
    {
      std::string filename;
      cout << "Enter input filename: ";
      cin >> filename;

      Parser inputParser(filename);
      data = inputParser.getData()[0];
    }
    else // from xbee
    {
      Receiver receiver("/dev/tty.usbserial-A501B6XH", 9600, 20, 1000);

      if (out) // to file
      {
        string filename;
        cout << "Enter output filename: ";
        cin >> filename;

        receiver.getData(filename.c_str());
        Parser inputParser(filename);
        data = inputParser.getData()[0];
      }
      else // not to file
      {
        data = receiver.getData();
      }
    }

    // Run data
		int id;
    std::string name;
		int grade;

    if (type == "PCC2D")
    {
      PCCTID pcctid;
      Parser parser("../data/base.dat");

      std::vector<std::vector<std::vector<int> > > dataTemp;
      dataTemp.push_back(data);

			id = pcctid.run(true, dataTemp);
      name = parser.getId()[id];
			grade = 10;

      if (print) pcctid.print(std::cout);
    }
    else if (type == "PCC3D")
    {
      PCCTID pcctid;
      Parser parser("../data/base.dat");

      std::vector<std::vector<std::vector<int> > > dataTemp;
      dataTemp.push_back(data);

			id = pcctid.run(false, dataTemp);
      name = parser.getId()[id];
			grade = 10;

      if (print) pcctid.print(std::cout);
    }
    else if (type == "MLPQuery")
    {
			// Prepare entry
			std::string filename;
			std::cout << "Enter entry name: ";
			std::cin >> filename;

			std::vector<int> yaw(data.size(), 0);
			std::vector<int> pitch(data.size(), 0);
			std::vector<int> roll(data.size(), 0);
			for (unsigned int i = 0; i < data.size(); i++)
			{
				yaw[i] = data[i][0];
				pitch[i] = data[i][1];
				roll[i] = data[i][2];
			}

			Entry entry(filename.c_str(), yaw, pitch, roll);

			// Add entry to index
			Index index;
			index.setEntry(entry);

			// Query network
      MLPTID mlptid;
			std::vector<double> result(mlptid.queryNetwork(entry.sampleIds, index.getEntryIds())); // TODO test results

			double resultMin = result[0];
			for (unsigned int i = 0; i < result.size(); i++)
			{
					std::cout << "resultMin = " << resultMin << std::endl;
					std::cout << "result = " << result[i] << std::endl;
				if (resultMin > result[i])
				{
					resultMin = result[i];
					id = i;
				}
			}

			name = index.getEntry(id).name; // TODO find bug
			grade = +10;

      if (print) 
			{          
				entry.print(std::cout);
				index.print(std::cout);
				mlptid.print(std::cout);
			}
    }
    else if (type == "MLPTrain")
    {
			// Prepare entry
			std::string filename;
			std::cout << "Enter entry name: ";
			std::cin >> filename;

			std::vector<int> yaw(data.size(), 0);
			std::vector<int> pitch(data.size(), 0);
			std::vector<int> roll(data.size(), 0);
			for (unsigned int i = 0; i < data.size(); i++)
			{
				yaw[i] = data[i][0];
				pitch[i] = data[i][1];
				roll[i] = data[i][2];
			}

			Entry entry(filename.c_str(), yaw, pitch, roll);

			// Add entry to index
			Index index;
			index.setEntry(entry);
			std::vector<int> trickIds = index.getEntryIds();

			// Query network
      MLPTID mlptid;
			mlptid.trainNetwork(entry.sampleIds, trickIds, entry.trickId);

			id = entry.trickId;
			name = entry.name;
			grade = -1;

      if (print) 
			{          
				entry.print(std::cout);
				index.print(std::cout);
				mlptid.print(std::cout);
			}
    }

    // Output results
    std::cout
        << std::endl
        << "--------------------------------------------------------" << std::endl
        << "class : Main" << std::endl
        << "field : Id, Name, Grade" << std::endl
        << "--------------------------------------------------------" << std::endl
        << std::endl;

    std::cout
				<< std::setw(20) << std::left
        << "Id"
				<< std::setw(20) << std::left
        << "Name"
				<< std::setw(20) << std::left
        << "Grade"
				<< std::endl
				<< std::setw(20) << std::left
				<< id
				<< std::setw(20) << std::left
				<< name
				<< std::setw(20) << std::left
				<< grade
				<< std::endl
				<< std::endl;
  }

  return 0;
}
