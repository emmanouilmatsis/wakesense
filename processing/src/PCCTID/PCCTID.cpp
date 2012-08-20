#include "PCCTID.h"

/* ======== PCCTID ======== */

/* -------- Public -------- */

PCCTID :: PCCTID()
  : baseFilename("/Users/em/Documents/Imperial/Dissertation/implementation/processing/data/base.dat")
{
  // Parse base file
  Parser baseParser(baseFilename);

  // Get parsed base data
  baseData = baseParser.getData();
  baseDataHeigth = baseParser.getData().size();
  baseDataWidth = baseParser.getData()[0].size();
  baseDataDepth = baseParser.getData()[0][0].size();
}

/* ---------------- */

PCCTID :: PCCTID(std::string baseFilename)
  : baseFilename(baseFilename)
{
  // Parse base file
  Parser baseParser(baseFilename);

  // Get parsed base data
  baseData = baseParser.getData();
  baseDataHeigth = baseParser.getData().size();
  baseDataWidth = baseParser.getData()[0].size();
  baseDataDepth = baseParser.getData()[0][0].size();
}

/* ---------------- */

void PCCTID :: run(bool type)
{
	// Get input filename
	std::string inputFilename;
  std::cout << "Enter input filename: ";
	std::cin >> inputFilename;

  // Parse input file
  Parser inputParser(inputFilename);

  // Get parsed input data
  inputData = inputParser.getData();

  // Verify equal number of samples
  if (inputData[0].size() != baseData[0].size())
    exit(1);

  // Identify set
	if (type)
		identifySet2D();
	else
		identifySet3D();
}

/* ---------------- */

void PCCTID :: run(bool type, std::vector<std::vector<std::vector<int> > > inputData)
{
  // Verify equal number of samples
  if (inputData[0].size() != baseData[0].size())
    exit(1);

  // Get parsed input data
  this->inputData = inputData;

  // Identify set
	if (type)
		identifySet2D();
	else
		identifySet3D();
}

/* ---------------- */

int PCCTID :: getId()
{
	return correlationMaxIndex;
}

/* ---------------- */

double PCCTID :: getCorrelation()
{
	return correlationMax;
}

/* ---------------- */

void PCCTID :: print(std::ostream& out)
{
  out.setf(std::ios::showpos);
  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : PCCTID" << std::endl
      << "field : Id, Correlation, InputData" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl;
	out
			<< "Id : " << correlationMaxIndex << std::endl
			<< "Correlation : " << correlationMax << std::endl
			<< std::endl;
  for (unsigned int i = 0; i < baseDataWidth; i++)
  {
    out
        << std::setw(2) << std::left
        << i
        << std::setw(5) << std::left
        << " = {"
        << std::setw(5) << std::left
        << inputData[0][i][0]
        << std::setw(5) << std::left
        << inputData[0][i][1]
        << std::setw(5) << std::left
        << inputData[0][i][2]
        << std::setw(10) << std::left
        << '}';

    if (!((i + 1) % 5))
      out << std::endl;
  }
  out << std::endl;

  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : PCCTID" << std::endl
      << "field : BaseData" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl;
  for (unsigned int i = 0; i < baseDataHeigth; i++)
  {
    for (unsigned int j = 0; j < baseDataWidth; j++)
    {
      out
          << std::setw(2) << std::left
          << j
          << std::setw(5) << std::left
          << " = {"
          << std::setw(5) << std::left
          << baseData[i][j][0]
          << std::setw(5) << std::left
          << baseData[i][j][1]
          << std::setw(5) << std::left
          << baseData[i][j][2]
          << std::setw(10) << std::left
          << '}';

      if (!((j + 1) % 5))
        out << std::endl;
    }
    if (i < baseDataHeigth - 1)
      out << std::endl;
  }
  out << std::endl;
}

/* -------- Private -------- */

void PCCTID :: identifySet2D()
{
  // Initialize input data
  std::vector<int> inputDataTemp(baseDataWidth, 0);

  for (unsigned int i = 0; i < baseDataWidth; i++)
    inputDataTemp[i] = std::sqrt(std::pow(static_cast<double>(inputData[0][i][0]), 2) + std::pow(static_cast<double>(inputData[0][i][1]), 2) + std::pow(static_cast<double>(inputData[0][i][2]), 2));

  // Initialize base data
  std::vector<std::vector<int> > baseDataTemp(baseDataHeigth, std::vector<int>(baseDataWidth, 0));

  for (unsigned int i = 0; i < baseDataHeigth; i++)
    for (unsigned int j = 0; j < baseDataWidth; j++)
      baseDataTemp[i][j] = std::sqrt(std::pow(static_cast<double>(baseData[i][j][0]), 2) + std::pow(static_cast<double>(baseData[i][j][1]), 2) + std::pow(static_cast<double>(baseData[i][j][2]), 2));

  // Initialize correlation max
  correlationMaxIndex = 0;
  correlationMax = pcc.calculate(inputDataTemp, baseDataTemp[0]);

  // Correlate input data to each column of base data
  for (unsigned int i = 0; i < baseDataHeigth; i++)
  {
    // Correlate input data to column i of base data
    double correlation = pcc.calculate(inputDataTemp, baseDataTemp[i]);

    // Update most correlated set index
    if (correlationMax < correlation)
    {
      correlationMax = correlation;
      correlationMaxIndex = i;
    }
  }
}

/* ---------------- */

void PCCTID :: identifySet3D()
{
  // Initialize input data
  std::vector<std::vector<int> > inputDataTemp(baseDataDepth, std::vector<int>(baseDataWidth, 0));

  for (unsigned int i = 0; i < baseDataDepth; i++)
    for (unsigned int k = 0; k < baseDataWidth; k++)
      inputDataTemp[i][k] = inputData[0][k][i];

  // Initialize base data
  std::vector<std::vector<std::vector<int> > > baseDataTemp(baseDataHeigth, std::vector<std::vector<int> >(baseDataDepth, std::vector<int>(baseDataWidth, 0)));

  for (unsigned int i = 0; i < baseDataHeigth; i++)
    for (unsigned int j = 0; j < baseDataDepth; j++)
      for (unsigned int k = 0; k < baseDataWidth; k++)
        baseDataTemp[i][j][k] =	baseData[i][k][j];

  // Initialize correlation max
  correlationMaxIndex = 0;
  correlationMax = (
                     pcc.calculate(inputDataTemp[0], baseDataTemp[0][0]) +
                     pcc.calculate(inputDataTemp[1], baseDataTemp[0][1]) +
                     pcc.calculate(inputDataTemp[2], baseDataTemp[0][2])) / 3;

  // Correlate input data to each column of base data
  for (unsigned int i = 0; i < baseDataHeigth; i++)
  {
    // Correlate input data to column i of base data
    double correlation = (
                           pcc.calculate(inputDataTemp[0], baseDataTemp[i][0]) +
                           pcc.calculate(inputDataTemp[1], baseDataTemp[i][1]) +
                           pcc.calculate(inputDataTemp[2], baseDataTemp[i][2])) / 3;

    // Update most correlated set index
    if (correlationMax < correlation)
    {
      correlationMaxIndex = i;
      correlationMax = correlation;
    }
  }
}
