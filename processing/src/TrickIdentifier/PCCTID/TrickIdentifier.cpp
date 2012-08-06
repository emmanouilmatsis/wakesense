#include "TrickIdentifier.h"

/* ======== TrickIdentifier ======== */

/* -------- Public -------- */

TrickIdentifier :: TrickIdentifier() {
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

TrickIdentifier :: TrickIdentifier(const char* inputFilename, const char* baseFilename) {
	// Allocate file names
	this->inputFilename = new char[strlen(inputFilename) + 1];
	this->baseFilename = new char[strlen(baseFilename) + 1];

	// Copy file names
	strcpy(this->inputFilename, inputFilename);
	strcpy(this->baseFilename, baseFilename);
}

TrickIdentifier :: TrickIdentifier(const TrickIdentifier& object) {
	// Allocate file names
	inputFilename = new char[strlen(object.inputFilename) + 1];
	baseFilename = new char[strlen(object.baseFilename) + 1];

	// Copy file names
	strcpy(inputFilename, object.inputFilename);
	strcpy(baseFilename, object.baseFilename);
}

TrickIdentifier :: ~TrickIdentifier() {
	// Deallocate file names
	delete[] inputFilename;
	delete[] baseFilename;
}

void TrickIdentifier :: operator =(const TrickIdentifier& object) {
	// Deallocate file names
	delete[] inputFilename;
	delete[] baseFilename;
}

int TrickIdentifier :: run() {
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

/* ======== TrickIdentifier2D ======== */

/* -------- Public -------- */

TrickIdentifier2D :: TrickIdentifier2D() : TrickIdentifier() {
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

TrickIdentifier2D :: TrickIdentifier2D(const char* inputFilename, const char* baseFilename) : TrickIdentifier(inputFilename, baseFilename) {
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

TrickIdentifier2D :: TrickIdentifier2D(const TrickIdentifier2D& object) : TrickIdentifier(object) {
	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

TrickIdentifier2D :: ~TrickIdentifier2D() {
	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);
}

void TrickIdentifier2D :: operator =(const TrickIdentifier2D& object) {
	// Invoke superclass operator =
	TrickIdentifier :: operator =(object);

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

void TrickIdentifier2D :: allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (heigth > 0) {
		target = new double**;
		target[0] = new double*[heigth];
		for (unsigned int i = 0; i < heigth; i++) {
			target[0][i] = new double[width];
		}
	}
}

void TrickIdentifier2D :: deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (target != NULL) {
		for (unsigned int i = 0; i < heigth; i++) {
			delete[] target[0][i];                                       
		}
		delete[] target[0];
		delete target;
	}
}

void TrickIdentifier2D :: copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	// Natural copy
	if (source != NULL) {
		for (unsigned int i = 0; i < heigth; i++) {
			for (unsigned int j = 0; j < width; j++) {
				target[0][i][j] = std :: sqrt(std :: pow(source[i][j][0], 2) + std :: pow(source[i][j][1], 2) + std :: pow(source[i][j][2], 2));
			}
		}
	}
}

int TrickIdentifier2D :: identifySet(double*** source) {
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

/* ======== TrickIdentifier3D ======== */

/* -------- Public -------- */

TrickIdentifier3D :: TrickIdentifier3D() : TrickIdentifier() {
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

TrickIdentifier3D :: TrickIdentifier3D(const char* inputFilename, const char* baseFilename) : TrickIdentifier(inputFilename, baseFilename) {
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

TrickIdentifier3D :: TrickIdentifier3D(const TrickIdentifier3D& object) : TrickIdentifier(object) {
	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

TrickIdentifier3D :: ~TrickIdentifier3D() {
	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);
}

void TrickIdentifier3D :: operator =(const TrickIdentifier3D& object) {
	// Invoke superclass operator =
	TrickIdentifier :: operator =(object);

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

void TrickIdentifier3D :: allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
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

void TrickIdentifier3D :: deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
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

void TrickIdentifier3D :: copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
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

int TrickIdentifier3D :: identifySet(double*** source) {
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
