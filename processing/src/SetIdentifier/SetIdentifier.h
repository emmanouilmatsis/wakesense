#ifndef SETIDENTIFIER_H
#define SETIDENTIFIER_H

#include <iostream>
#include <cstring>
#include <cmath>
#include "../Parser/Parser.h"
#include "../PearsonCorrelation/PearsonCorrelation.h"

class SetIdentifier {
	public:
		SetIdentifier();
		SetIdentifier(const char* inputFileName, const char* baseFileName);
		SetIdentifier(const SetIdentifier& object);
		virtual ~SetIdentifier();
		void operator =(const SetIdentifier& object);
		int run();

	protected:
		char* inputFilename;
		char* baseFilename;
		double*** baseData;
		unsigned int baseDataRowSize;
		unsigned int baseDataColumnSize;
		PearsonCorrelation<double> pearsonCorrelation;

		virtual void allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) = 0;
		virtual void deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) = 0;
		virtual void copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) = 0;
		virtual int identifySet(double*** source) = 0;
};

class SetIdentifier2D : public SetIdentifier {
	public:
		SetIdentifier2D();
		SetIdentifier2D(const char* inputFilename, const char* baseFilename);
		SetIdentifier2D(const SetIdentifier2D& object);
		~SetIdentifier2D();
		void operator =(const SetIdentifier2D& object);

	private:
		void allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth);
		void deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth);
		void copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth);
		int identifySet(double*** source);
};

class SetIdentifier3D : public SetIdentifier {
	public:
		SetIdentifier3D();
		SetIdentifier3D(const char* inputFilename, const char* baseFilename);
		SetIdentifier3D(const SetIdentifier3D& object);
		~SetIdentifier3D();
		void operator =(const SetIdentifier3D& object);

	private:
		void allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth);
		void deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth);
		void copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth);
		int identifySet(double*** source);
};

#endif // SETIDENTIFIER_H

/*
#if EUCLIDEAN

double*** tempData = parser.getData();
int columnSize = parser.getColumnSize();
int rowSize = parser.getRowSize();

double** data = new double*[rowSize];
for (int i = 0; i < rowSize; i++) {
	data[i] = new double[columnSize];
	for (int j = 0; j < columnSize; j++) {
		data[i][j] = std :: sqrt(std :: pow(tempData[i][j][0], 2) + std :: pow(tempData[i][j][1], 2) + std :: pow(tempData[i][j][2], 2));
	}
}

std :: cout.setf(std :: ios :: showpoint);
std :: cout.setf(std :: ios :: fixed);
std :: cout.precision(2);

int corrMaxPair[2] = {0, 1};
double corrMax = pc.calculate(data[0], data[1], columnSize);
for (int i = 0; i < rowSize; i++) {
	for (int j = i + 1; j < rowSize; j++) {
		double corr = pc.calculate(data[i], data[j], columnSize);
		if (corr > corrMax) {
			corrMax = corr;
			corrMaxPair[0] = i;
			corrMaxPair[1] = j;
		}

		std :: cout << "i = " << i;
		std :: cout << ", j = " << j;
		std :: cout << ", corr = " << corr;
		std :: cout << std :: endl;
	}
}
std :: cout << "Most correlated pair: " << corrMaxPair[0] << " - " << corrMaxPair[1];
std :: cout << std :: endl;

#else

double*** tempData = parser.getData();
int columnSize = parser.getColumnSize();
int rowSize = parser.getRowSize();

double** yaw = new double*[rowSize];
double** pitch = new double*[rowSize];
double** roll = new double*[rowSize];
for (int i = 0; i < rowSize; i++) {
	yaw[i] = new double[columnSize];
	pitch[i] = new double[columnSize];
	roll[i] = new double[columnSize];
	for (int j = 0; j < columnSize; j++) {
		yaw[i][j] = tempData[i][j][0];
		pitch[i][j] = tempData[i][j][1];
		roll[i][j] = tempData[i][j][2];
	}
}

std :: cout.setf(std :: ios :: showpoint);
std :: cout.setf(std :: ios :: fixed);
std :: cout.precision(5);

int corrMaxPair[2] = {0, 1};
double corrMax = (pc.calculate(yaw[0], yaw[1], columnSize) + pc.calculate(pitch[0], pitch[1], columnSize) + pc.calculate(roll[0], roll[1], columnSize)) / 3;
for (int i = 0; i < rowSize; i++) {
	for (int j = i + 1; j < rowSize; j++) {
		double corr = (pc.calculate(yaw[i], yaw[j], columnSize) + pc.calculate(pitch[i], pitch[j], columnSize) + pc.calculate(roll[i], roll[j], columnSize)) / 3;
		if (corr > corrMax) {
			corrMax = corr;
			corrMaxPair[0] = i;
			corrMaxPair[1] = j;
		}

		std :: cout << "i = " << i << ", j = " << j;
		std :: cout << ", corrYaw = " << pc.calculate(yaw[i], yaw[j], columnSize);
		std :: cout << ", corrPitch = " << pc.calculate(pitch[i], pitch[j], columnSize);
		std :: cout << ", corrRoll = " << pc.calculate(roll[i], roll[j], columnSize);
		std :: cout << ", corrAverage = " << corr;
		std :: cout << std :: endl;
	}
}
std :: cout << "Most correlated pair: " << corrMaxPair[0] << " - " << corrMaxPair[1];
std :: cout << std :: endl;

#endif


return 0;
}
*/