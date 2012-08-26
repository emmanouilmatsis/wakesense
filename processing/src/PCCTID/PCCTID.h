#ifndef PCCTID_H
#define PCCTID_H

#include <ostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include "../TrickDatabase/TrickDatabase.h"
#include "../PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h"

#include <iostream>

class PCCTID {
	public:
		PCCTID(std::string filename);
		void run2D(std::vector<std::vector<int> > inputData);
		void run3D(std::vector<std::vector<int> > inputData);
		std::string getName();
		double getCorrelation();
		friend std::ostream& operator <<(std::ostream& out, const PCCTID& object);

	protected:
		TrickDatabase trickDatabase;
		std::vector<std::vector<std::vector<int> > > baseData;
		unsigned int baseDataHeigth;
		unsigned int baseDataWidth;
		unsigned int baseDataDepth;
		std::vector<std::vector<int> > inputData;
		int correlationMaxIndex;
		double correlationMax;
		PearsonCorrelationCoefficient<int> pcc;
};

#endif // PCCTID_H
