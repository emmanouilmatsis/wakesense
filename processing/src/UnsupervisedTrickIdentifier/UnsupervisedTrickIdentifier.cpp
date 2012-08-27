#include "UnsupervisedTrickIdentifier.h"

/* -------- Public -------- */

UnsupervisedTrickIdentifier :: UnsupervisedTrickIdentifier(std::string filename)
  : trickDatabase(filename)
{
  // Initialize base data
  int rowid = 1;
  Trick trick = trickDatabase.get(rowid);

  while (trick.name != "NULL")
  {
    baseData.push_back(std::vector<std::vector<int> >());

    for (unsigned int i = 0; i < trick.yaw.size(); i++)
    {
      baseData[rowid - 1].push_back(std::vector<int>(3));

      baseData[rowid - 1][i][0] = trick.yaw[i];
      baseData[rowid - 1][i][1] = trick.pitch[i];
      baseData[rowid - 1][i][2] = trick.roll[i];
    }

    trick = trickDatabase.get(++rowid);
  }

  baseDataHeigth = baseData.size();
  baseDataWidth = baseData[0].size();
  baseDataDepth = baseData[0][0].size();
}

/* ---------------- */

void UnsupervisedTrickIdentifier :: runPCC2D(std::vector<std::vector<int> > inputData)
{
  // Verify equal number of samples
  if (inputData.size() != baseData[0].size())
    exit(1);

  // Get parsed input data
  this->inputData = inputData;

  // Initialize input data
  std::vector<int> inputDataTemp(baseDataWidth, 0);

  for (unsigned int i = 0; i < baseDataWidth; i++)
    inputDataTemp[i] = std::sqrt(std::pow(static_cast<double>(inputData[i][0]), 2) + std::pow(static_cast<double>(inputData[i][1]), 2) + std::pow(static_cast<double>(inputData[i][2]), 2));

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

void UnsupervisedTrickIdentifier :: runPCC3D(std::vector<std::vector<int> > inputData)
{
  // Verify equal number of samples
  if (inputData.size() != baseData[0].size())
    exit(1);

  // Get parsed input data
  this->inputData = inputData;

  // Initialize input data
  std::vector<std::vector<int> > inputDataTemp(baseDataDepth, std::vector<int>(baseDataWidth, 0));

  for (unsigned int i = 0; i < baseDataDepth; i++)
    for (unsigned int k = 0; k < baseDataWidth; k++)
      inputDataTemp[i][k] = inputData[k][i];

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

/* ---------------- */

void UnsupervisedTrickIdentifier :: runSD(std::vector<std::vector<int> > inputData)
{
  // Verify equal number of samples
  if (inputData.size() != baseData[0].size())
    exit(1);

  // Get parsed input data
  this->inputData = inputData;

  // Initialize input data
  std::vector<std::vector<int> > inputDataTemp(baseDataDepth, std::vector<int>(baseDataWidth, 0));

  for (unsigned int i = 0; i < baseDataDepth; i++)
    for (unsigned int k = 0; k < baseDataWidth; k++)
      inputDataTemp[i][k] = inputData[k][i];

  // Standard diviation input data
  double inputYawSD = standardDiviation(inputDataTemp[0]);
  double inputPitchSD = standardDiviation(inputDataTemp[1]);
  double inputRollSD = standardDiviation(inputDataTemp[2]);

  // Initialize base data
  std::vector<std::vector<std::vector<int> > > baseDataTemp(baseDataHeigth, std::vector<std::vector<int> >(baseDataDepth, std::vector<int>(baseDataWidth, 0)));

  for (unsigned int i = 0; i < baseDataHeigth; i++)
    for (unsigned int j = 0; j < baseDataDepth; j++)
      for (unsigned int k = 0; k < baseDataWidth; k++)
        baseDataTemp[i][j][k] =	baseData[i][k][j];

	// Initialize 
  correlationMaxIndex = 0;

  // Standard diviation base data
  for (unsigned int i = 0; i < baseDataHeigth; i++)
  {
    int yawSD = abs(inputYawSD - standardDiviation(baseDataTemp[i][0]));
    int pitchSD = abs(inputPitchSD - standardDiviation(baseDataTemp[i][1]));
    int rollSD = abs(inputRollSD - standardDiviation(baseDataTemp[i][2]));

    if ((yawSD == 0) && (pitchSD == 0) && (rollSD == 0))
    {
      correlationMaxIndex = i;
			correlationMax = 1;
			return;
    }
  }
}

/* ---------------- */

double UnsupervisedTrickIdentifier :: standardDiviation(std::vector<int> input)
{
  double size = input.size();

  // Mean
  double mean = 0;
  for (unsigned int i = 0; i < input.size(); i++)
    mean += input[i];
  mean /= size;

  // Standard diviation
  double standardDiviation = 0;

  for (unsigned int i = 0; i < input.size(); i++)
    standardDiviation += ((input[i] - mean) * (input[i] - mean));
  standardDiviation = sqrt(standardDiviation / size);

  return standardDiviation;
}

/* ---------------- */

std::string UnsupervisedTrickIdentifier :: getName()
{
  return trickDatabase.get(correlationMaxIndex + 1).name;
}

/* ---------------- */

double UnsupervisedTrickIdentifier :: getCorrelation()
{
  return correlationMax;
}

/* ---------------- */

std::ostream& operator <<(std::ostream& out, UnsupervisedTrickIdentifier& object)
{
  out.setf(std::ios::showpos);
  out.setf(std::ios::fixed);
  out.precision(3);

  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : UnsupervisedTrickIdentifier" << std::endl
      << "field : Id, Correlation, InputData" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl;

  out
      << "Id : " << object.correlationMaxIndex << std::endl
      << "Correlation : " << object.correlationMax << std::endl
      << std::endl;

  for (unsigned int i = 0; i < object.baseDataWidth; i++)
  {
    out
        << std::setw(2) << std::left << i
        << std::setw(5) << std::left << " = {"
        << std::setw(5) << std::left << object.inputData[i][0]
        << std::setw(5) << std::left << object.inputData[i][1]
        << std::setw(5) << std::left << object.inputData[i][2]
        << std::setw(10) << std::left << '}';

    if (!((i + 1) % 5))
      out << std::endl;
  }
  out << std::endl;

  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : UnsupervisedTrickIdentifier" << std::endl
      << "field : BaseData" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl;

  for (unsigned int i = 0; i < object.baseDataHeigth; i++)
  {
		out 
				<< object.trickDatabase.get(i + 1).name << std::endl;
    for (unsigned int j = 0; j < object.baseDataWidth; j++)
    {
      out
          << std::setw(2) << std::left << j
          << std::setw(5) << std::left << " = {"
          << std::setw(5) << std::left << object.baseData[i][j][0]
          << std::setw(5) << std::left << object.baseData[i][j][1]
          << std::setw(5) << std::left << object.baseData[i][j][2]
          << std::setw(10) << std::left << '}';

      if (!((j + 1) % 5))
        out << std::endl;
    }
    if (i < object.baseDataHeigth - 1)
      out << std::endl;
  }
  out << std::endl;

  return out;
}
