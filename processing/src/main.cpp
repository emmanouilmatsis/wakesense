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
#include "Score/Score.h"

/* -------- Constant -------- */

#define MIN_ARGC 2
#define DEFAULT_DATABASE_FILENAME "trickDatabase.db"
#define PORT "/dev/tty.usbserial-A501B6XH"
#define BAUD 9600
#define INTERVAL 20
#define DURATION 1000

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

  std::vector<std::vector<int> > data;
  Score score(DEFAULT_DATABASE_FILENAME);

  while (true)
  {
    // Get data
    if (in) // from file
    {
      std::string filename;
      std::cout << "Enter input filename: ";
      std::cin >> filename;

      std::cout << "Parsing data..." << std::endl;
      Parser inputParser(filename);
      data = inputParser.getData()[0]; // get the first row of the matrix
    }
    else // from xbee
    {
      Receiver receiver(PORT, BAUD, INTERVAL, DURATION);

      if (out) // to file
      {
        std::string filename;
        std::cout << "Enter output filename: ";
        std::cin >> filename;

        std::cout << "Receiving data..." << std::endl;
        receiver.getData(filename.c_str());

        Parser inputParser(filename);
        data = inputParser.getData()[0];
      }
      else // not to file
      {
        std::cout << "Receiving data..." << std::endl;
        data = receiver.getData();
      }
    }

		// Run data
    if ((type == "PCC2D") || (type == "PCC3D"))
    {
      // Run PCCTID2D
      PCCTID pcctid(DEFAULT_DATABASE_FILENAME);

			if (type == "PCC2D")
				pcctid.run2D(data);
			else 
				pcctid.run3D(data);

      // Get score
      score.run(pcctid.getName(), pcctid.getCorrelation(), data);
      std::cout
					<< score
          << std::endl;

      // Print results
      if (print)
        std::cout
            << pcctid
            << std::endl;
    }
    else if (type == "MLPQuery")
    {
      // Format data
      std::vector<int> yaw(data.size(), 0);
      std::vector<int> pitch(data.size(), 0);
      std::vector<int> roll(data.size(), 0);
      for (unsigned int i = 0; i < data.size(); i++)
      {
        yaw[i] = data[i][0];
        pitch[i] = data[i][1];
        roll[i] = data[i][2];
      }

      // Construct entry
      Entry entry("QUERY", yaw, pitch, roll);

      // Add entry to index
      Index index;
      index.setEntry(entry);

      // Query network
      MLPTID mlptid;
      mlptid.queryNetwork(entry.sampleIds, index.getEntryIds());

      // Get score
      std::string name = index.getEntry(index.getEntryIds()[mlptid.getId()]).name;
      score.run(name, mlptid.getCorrelation(), data);
      std::cout
					<< score
          << std::endl;

      // Print results
      if (print)
        std::cout
            << entry
            << index
            << mlptid
            << std::endl;
    }
    else if (type == "MLPTrain")
    {
      // Get entry name
			std::string name;
      std::cout << "Enter entry name: ";
      std::cin >> name;

      // Format data
      std::vector<int> yaw(data.size(), 0);
      std::vector<int> pitch(data.size(), 0);
      std::vector<int> roll(data.size(), 0);
      for (unsigned int i = 0; i < data.size(); i++)
      {
        yaw[i] = data[i][0];
        pitch[i] = data[i][1];
        roll[i] = data[i][2];
      }

      // Construct entry
      Entry entry(name.c_str(), yaw, pitch, roll);

      // Add entry to index
      Index index;
      index.setEntry(entry);
      std::vector<int> trickIds = index.getEntryIds();

      // Train network
      MLPTID mlptid;
      mlptid.trainNetwork(entry.sampleIds, trickIds, entry.trickId);

      // Print results
      if (print)
        std::cout
            << entry
            << index
            << mlptid
            << std::endl;
    }
  }

  return 0;
}
