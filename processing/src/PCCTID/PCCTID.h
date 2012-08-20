#ifndef PCCTID_H
#define PCCTID_H

#include <iostream>
#include <ostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include "../Parser/Parser.h"
#include "../PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h"

/* --------- Class declaration -------- */

class PCCTID {
	public:
		PCCTID();
		PCCTID(std::string baseFileName);
		void run(bool type);
		void run(bool type, std::vector<std::vector<std::vector<int> > > inputData);
		int getId();
		double getCorrelation();
		void print(std::ostream& out);

	protected:
		std::string baseFilename;
		std::vector<std::vector<std::vector<int> > > baseData;
		unsigned int baseDataHeigth;
		unsigned int baseDataWidth;
		unsigned int baseDataDepth;
		std::vector<std::vector<std::vector<int> > > inputData;
		int correlationMaxIndex;
		double correlationMax;
		PearsonCorrelationCoefficient<int> pcc;

		void identifySet2D();
		void identifySet3D();
};

#endif // PCCTID_H
