#include "SetIdentifier.h"

/* ======== SetIdentifier ======== */

/* -------- Public -------- */

SetIdentifier :: SetIdentifier() {
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

SetIdentifier :: SetIdentifier(const char* inputFilename, const char* baseFilename) {
	// Allocate file names
	this->inputFilename = new char[strlen(inputFilename) + 1];
	this->baseFilename = new char[strlen(baseFilename) + 1];

	// Copy file names
	strcpy(this->inputFilename, inputFilename);
	strcpy(this->baseFilename, baseFilename);
}

SetIdentifier :: SetIdentifier(const SetIdentifier& object) {
	// Allocate file names
	inputFilename = new char[strlen(object.inputFilename) + 1];
	baseFilename = new char[strlen(object.baseFilename) + 1];

	// Copy file names
	strcpy(inputFilename, object.inputFilename);
	strcpy(baseFilename, object.baseFilename);
}

SetIdentifier :: ~SetIdentifier() {
	// Deallocate file names
	delete[] inputFilename;
	delete[] baseFilename;
}

void SetIdentifier :: operator =(const SetIdentifier& object) {
	// Deallocate file names
	delete[] inputFilename;
	delete[] baseFilename;
}

int SetIdentifier :: run() {
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

/* ======== SetIdentifier2D ======== */

/* -------- Public -------- */

SetIdentifier2D :: SetIdentifier2D() : SetIdentifier() {
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

SetIdentifier2D :: SetIdentifier2D(const char* inputFilename, const char* baseFilename) : SetIdentifier(inputFilename, baseFilename) {
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

SetIdentifier2D :: SetIdentifier2D(const SetIdentifier2D& object) : SetIdentifier(object) {
	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

SetIdentifier2D :: ~SetIdentifier2D() {
	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);
}

void SetIdentifier2D :: operator =(const SetIdentifier2D& object) {
	// Invoke superclass operator =
	SetIdentifier :: operator =(object);

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

void SetIdentifier2D :: allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (heigth > 0) {
		target = new double**[heigth];
		for (unsigned int i = 0; i < heigth; i++) {
			target[i] = new double*[width];
			for (unsigned int j = 0; j < width; j++) {
				target[i][j] = new double;
			}
		}
	}
}

void SetIdentifier2D :: deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	if (target != NULL) {
		for (unsigned int i = 0; i < heigth; i++) {
			for (unsigned int j = 0; j < width; j++) {
				delete target[i][j];                                       
			}
			delete[] target[i];
		}
		delete[] target;
	}
}

void SetIdentifier2D :: copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
	// Natural copy
	if (source != NULL) {
		for (unsigned int i = 0; i < heigth; i++) {
			for (unsigned int j = 0; j < width; j++) {
				*target[i][j] = std :: sqrt(std :: pow(source[i][j][0], 2) + std :: pow(source[i][j][1], 2) + std :: pow(source[i][j][2], 2));
			}
		}
	}
}

int SetIdentifier2D :: identifySet(double*** source) {
	// Convert 3D input data to 1D input data
	double* inputDataColumn = new double[baseDataColumnSize];
	for (unsigned int j = 0; j < baseDataColumnSize; j++)
		inputDataColumn[j] = *source[0][j];

	// Initialize correlation max
	int setIndex = 0;
	double* baseDataColumnInitial = new double[baseDataColumnSize];
	for (unsigned int j = 0; j < baseDataColumnSize; j++)
		baseDataColumnInitial[j] = *baseData[setIndex][j];
	double correlationMax = pearsonCorrelation.calculate(inputDataColumn, baseDataColumnInitial, baseDataColumnSize);

	delete[] baseDataColumnInitial;

	// Correlate input data to each column of base data
	for (unsigned int i = 0; i < baseDataRowSize; i++) {

		// Convert 3D base data to 1D base data
		double* baseDataColumn = new double[baseDataColumnSize];
		for (unsigned int j = 0; j < baseDataColumnSize; j++)
			baseDataColumn[j] = *baseData[i][j];

		// Correlate input data to column i of base data
		double correlation = pearsonCorrelation.calculate(inputDataColumn, baseDataColumn, baseDataColumnSize);

		// Update most correlated set index
		if (correlationMax < correlation) {
			correlationMax = correlation;
			setIndex = i;
		}

		delete[] baseDataColumn;
	}
	delete[] inputDataColumn;

	return setIndex;
}

/* ======== SetIdentifier3D ======== */

/* -------- Public -------- */

SetIdentifier3D :: SetIdentifier3D() : SetIdentifier() {
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

SetIdentifier3D :: SetIdentifier3D(const char* inputFilename, const char* baseFilename) : SetIdentifier(inputFilename, baseFilename) {
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

SetIdentifier3D :: SetIdentifier3D(const SetIdentifier3D& object) : SetIdentifier(object) {
	// Initialize data size
	baseDataRowSize = object.baseDataRowSize;
	baseDataColumnSize = object.baseDataColumnSize;

	// Allocate base data
	allocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);

	// Copy base data
	copyData(object.baseData, baseData, baseDataRowSize, baseDataColumnSize, 3);
}

SetIdentifier3D :: ~SetIdentifier3D() {
	// Deallocate base data
	deallocateData(baseData, baseDataRowSize, baseDataColumnSize, 3);
}

void SetIdentifier3D :: operator =(const SetIdentifier3D& object) {
	// Invoke superclass operator =
	SetIdentifier :: operator =(object);

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

void SetIdentifier3D :: allocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
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

void SetIdentifier3D :: deallocateData(double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
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

void SetIdentifier3D :: copyData(double*** source, double***& target, unsigned int heigth, unsigned int width, unsigned int depth) {
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

int SetIdentifier3D :: identifySet(double*** source) {
	// Initialize correlation max
	int setIndex = 0;
	double correlationMax = (
			pearsonCorrelation.calculate(source[0][0], baseData[0][0], baseDataColumnSize) +
			pearsonCorrelation.calculate(source[1][0], baseData[1][0], baseDataColumnSize) +
			pearsonCorrelation.calculate(source[2][0], baseData[2][0], baseDataColumnSize)) / 3;

	// Correlate input data to each column of base data
	for (unsigned int i = 0; i < baseDataRowSize; i++) {

		// Correlate input data to column i of base data
		double correlation = (
				pearsonCorrelation.calculate(source[0][0], baseData[0][i], baseDataColumnSize) +
				pearsonCorrelation.calculate(source[1][0], baseData[1][i], baseDataColumnSize) +
				pearsonCorrelation.calculate(source[2][0], baseData[2][i], baseDataColumnSize)) / 3;

		// Update most correlated set index
		if (correlationMax < correlation) {
			correlationMax = correlation;
			setIndex = i;
		}
	}

	return setIndex;
}