#ifndef PCCTID_H
#define PCCTID_H

#include <ostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include "../Parser/Parser.h"
#include "../PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h"


#include <iostream>

/* --------- Type definition -------- */

typedef std::vector<std::vector<std::vector<int> > > Data3D;

/* --------- Class declaration -------- */

class PCCTID {
	public:
		PCCTID();
		PCCTID(std::string inputFileName, std::string baseFileName);
		int run(bool type);
		int run(bool type, std::vector<std::vector<std::vector<int> > > inputData);
		void print(std::ostream& out);

	protected:
		std::string inputFilename;
		std::string baseFilename;
		std::vector<std::vector<std::vector<int> > > inputData;
		std::vector<std::vector<std::vector<int> > > baseData;
		unsigned int baseDataHeigth;
		unsigned int baseDataWidth;
		unsigned int baseDataDepth;
		int setIndex; // TODO return it
		double correlationMax; // TODO return it
		PearsonCorrelationCoefficient<int> pcc;

		int identifySet2D();
		int identifySet3D();
};

#endif // PCCTID_H
