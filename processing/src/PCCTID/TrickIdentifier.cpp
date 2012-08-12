#include "PCCTID.h"

/* ======== PCCTID ======== */

/* -------- Public -------- */

PCCTID :: PCCTID() {
	// Default file names
	const char* inputFilenameDefault = "/Users/em/Documents/Imperial/Dissertation/implementation/processing/data/input.dat";
	const char* baseFilenameDefault = "/Users/em/Documents/Imperial/Dissertation/implementation/processing/data/base.dat";

	// Allocate file names
	inputFilename = new char[strlen(inputFilenameDefault) + 1];
	baseFilename = new char[strlen(baseFilenameDefault) + 1];

	// Copy file names
	strcpy(inputFilename, inputFilenameDefault);
	strcpy(baseFilename, baseFilenameDefault);
}

PCCTID :: PCCTID(const char* inputFilename, const char* baseFilename) {
	// Allocate file names
	this->inputFilename = new char[strlen(inputFilename) + 1];
	this->baseFilename = new char[strlen(baseFilename) + 1];

	// Copy file names
	strcpy(this->inputFilename, inputFilename);
	strcpy(this->baseFilename, baseFilename);
}

PCCTID :: PCCTID(const PCCTID& object) {
	// Allocate file names
	inputFilename = new char[strlen(object.inputFilename) + 1];
	baseFilename = new char[strlen(object.baseFilename) + 1];

	// Copy file names
	strcpy(inputFilename, object.inputFilename);
	strcpy(baseFilename, object.baseFilename);
}

PCCTID :: ~PCCTID() {
	// Deallocate file names
	delete[] inputFilename;
	delete[] baseFilename;
}

void PCCTID :: operator =(const PCCTID& object) {
	// Deallocate file names
	delete[] inputFilename;
	delete[] baseFilename;
}

int PCCTID :: run() {
	// Parse default input file
	Parser inputParser(inputFilename);

	// Verify equal number of samples
	if (baseDataColumnSize != inputParser.getColumnSize())
		exit(1);

	// Allocate input data
	double*** inputData;
	allocateData(inputData, 1, baseDataColumnSize, 3);

	// Copy input data
	copyData(inputParser.getData(), inputData, 1, baseDataColumnSize, 3);

	// Identify set
	int setIndex = identifySet(inputData);

	// Deallocate data
	deallocateData(inputData, 1, baseDataColumnSize, 3);

	return setIndex;
}

/* ======== PCCTID2D ======== */

/* -------- Public -------- */

PCCTID2D :: PCCTID2D() : PCCTID() {
	// Parse default base file
	Parser baseParser(baseFilename);

	// Initialize data size
	baseDataRowSize = baseParser.getRowSize();
	baseDataColumnSize = baseParser.getColumnSize();

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(baseParser.getData(), baseData, baseDataRowSize, baseDataColumnSize, 3);
}

PCCTID2D :: PCCTID2D(const char* inputFilename, const char* baseFilename) : PCCTID(inputFilename, baseFilename) {
	// Parse base file
	Parser baseParser(baseFilename);


	// Initialize data size
	baseDataRowSize = baseParser.getRowSize();
	baseDataColumnSize = baseParser.getColumnSize();

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(baseParser.getData(), baseData, baseDataRowSize, baseDataColumnSize, 3);
}

PCCTID2D :: PCCTID2D(const PCCTID2D& object) : PCCTID(object) {
	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

PCCTID2D :: ~PCCTID2D() {
	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);
}

void PCCTID2D :: operator =(const PCCTID2D& object) {
	// Invoke superclass operator =
	PCCTID :: operator =(object);

	// Allocate file names
	char* inputFilenameTemp = new char[strlen(object.inputFilename) + 1];
	strcpy(inputFilenameTemp, object.inputFilename);

	char* baseFilenameTemp = new char[strlen(object.baseFilename) + 1];
	strcpy(baseFilenameTemp, object.baseFilename);

	// Copy file names
	inputFilename = inputFilenameTemp;
	baseFilename = baseFilenameTemp;

	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

/* -------- Private -------- */

void PCCTID2D :: allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (heigth > 0) {
		target = new double**;
		target[0] = new double*[heigth];
		for (unsigned int i = 0; i < heigth; i++) {
			target[0][i] = new double[width];
		}
	}
}

void PCCTID2D :: deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (target != NULL) {
		for (unsigned int i = 0; i < heigth; i++) {
			delete[] target[0][i];                                       
		}
		delete[] target[0];
		delete target;
	}
}

void PCCTID2D :: copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	// Natural copy
	if (source != NULL) {
		for (unsigned int i = 0; i < heigth; i++) {
			for (unsigned int j = 0; j < width; j++) {
				target[0][i][j] = std :: sqrt(std :: pow(source[i][j][0], 2) + std :: pow(source[i][j][1], 2) + std :: pow(source[i][j][2], 2));
			}
		}
	}
}

int PCCTID2D :: identifySet(double*** source) {
	// Initialize correlation max
	int setIndex = 0;
	double correlationMax = pcc.calculate(source[0][0], baseData[0][0], baseDataColumnSize);

	// Correlate input data to each column of base data
	for (unsigned int i = 0; i < baseDataRowSize; i++) {

		// Correlate input data to column i of base data
		double correlation = pcc.calculate(source[0][0], baseData[0][i], baseDataColumnSize);

		// Update most correlated set index
		if (correlationMax < correlation) {
			correlationMax = correlation;
			setIndex = i;
		}
	}

	return setIndex;
}

/* ======== PCCTID3D ======== */

/* -------- Public -------- */

PCCTID3D :: PCCTID3D() : PCCTID() {
	// Parse default base file
	Parser baseParser(baseFilename);

	// Initialize data size
	baseDataRowSize = baseParser.getRowSize();
	baseDataColumnSize = baseParser.getColumnSize();

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(baseParser.getData(), baseData, baseDataRowSize, baseDataColumnSize, 3);
}

PCCTID3D :: PCCTID3D(const char* inputFilename, const char* baseFilename) : PCCTID(inputFilename, baseFilename) {
	// Parse base file
	Parser baseParser(baseFilename);

	// Initialize data size
	baseDataRowSize = baseParser.getRowSize();
	baseDataColumnSize = baseParser.getColumnSize();

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(baseParser.getData(), baseData, baseDataRowSize, baseDataColumnSize, 3);
}

PCCTID3D :: PCCTID3D(const PCCTID3D& object) : PCCTID(object) {
	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

PCCTID3D :: ~PCCTID3D() {
	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);
}

void PCCTID3D :: operator =(const PCCTID3D& object) {
	// Invoke superclass operator =
	PCCTID :: operator =(object);

	// Allocate file names
	char* inputFilenameTemp = new char[strlen(object.inputFilename) + 1];
	strcpy(inputFilenameTemp, object.inputFilename);

	char* baseFilenameTemp = new char[strlen(object.baseFilename) + 1];
	strcpy(baseFilenameTemp, object.baseFilename);

	// Copy file names
	inputFilename = inputFilenameTemp;
	baseFilename = baseFilenameTemp;

	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

/* -------- Private -------- */

void PCCTID3D :: allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (heigth > 0) {
		target = new double**[depth];
		for (unsigned int i = 0; i < depth; i++) {
			target[i] = new double*[heigth];
			for (unsigned int j = 0; j < heigth; j++) {
				target[i][j] = new double[width];
			}
		}
	}
}

void PCCTID3D :: deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (target != NULL) {
		for (unsigned int i = 0; i < depth; i++) {
			for (unsigned int j = 0; j < heigth; j++) {
				delete[] target[i][j];                                       
			}
			delete[] target[i];
		}
		delete[] target;
	}
}

void PCCTID3D :: copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	// Pivot copy
	if (source != NULL) {
		for (unsigned int i = 0; i < depth; i++) {
			for (unsigned int j = 0; j < heigth; j++) {
				for (unsigned int k = 0; k < width; k++) {
					target[i][j][k] = source[j][k][i];
				}
			}
		}
	}
}

int PCCTID3D :: identifySet(double*** source) {
	// Initialize correlation max
	int setIndex = 0;
	double correlationMax = (
			pcc.calculate(source[0][0], baseData[0][0], baseDataColumnSize) +
			pcc.calculate(source[1][0], baseData[1][0], baseDataColumnSize) +
			pcc.calculate(source[2][0], baseData[2][0], baseDataColumnSize)) / 3;

	// Correlate input data to each column of base data
	for (unsigned int i = 0; i < baseDataRowSize; i++) {

		// Correlate input data to column i of base data
		double correlation = (
				pcc.calculate(source[0][0], baseData[0][i], baseDataColumnSize) +
				pcc.calculate(source[1][0], baseData[1][i], baseDataColumnSize) +
				pcc.calculate(source[2][0], baseData[2][i], baseDataColumnSize)) / 3;

		// Update most correlated set index
		if (correlationMax < correlation) {
			correlationMax = correlation;
			setIndex = i;
		}
	}

	return setIndex;
}
