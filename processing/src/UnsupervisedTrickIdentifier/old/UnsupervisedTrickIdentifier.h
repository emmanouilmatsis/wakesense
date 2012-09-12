#ifndef UNSUPERVISEDTRICKIDENTIFIER_H
#define UNSUPERVISEDTRICKIDENTIFIER_H

#include <ostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include "../TrickDatabase/TrickDatabase.h"
#include "../PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h"
#include "../EuclideanDistance/EuclideanDistance.h"

#include <iostream>
using std::cout;
using std::endl;

class UnsupervisedTrickIdentifier {
	public:
		UnsupervisedTrickIdentifier(std::string filename);
		void runPCC2D(std::vector<std::vector<int> > inputData);
		void runPCC3D(std::vector<std::vector<int> > inputData);
		void runSD(std::vector<std::vector<int> > inputData);
		void runED(std::vector<std::vector<int> > inputData);
		std::string getName();
		double getCorrelation();
		friend std::ostream& operator <<(std::ostream& out, UnsupervisedTrickIdentifier& object);

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
		EuclideanDistance<int> ed;
		double standardDiviation(std::vector<int> input);
};

#endif // UNSUPERVISEDTRICKIDENTIFIER_H
