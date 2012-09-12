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

void UnsupervisedTrickIdentifier :: run(std::vector<std::vector<int> > inputData)
{
  // Verify equal number of samples
  if (inputData.size() != baseData[0].size())
    exit(1);

  // Get input data
  this->inputData = inputData;

  // Format input data
  std::vector<std::vector<int> > inputDataTemp(baseDataDepth, std::vector<int>(baseDataWidth, 0));

  for (unsigned int i = 0; i < baseDataDepth; i++)
  {
    inputDataTemp[i][0] = inputData[0][i];

    for (unsigned int j = 1; j < baseDataWidth; j++)
    {
      // Non-transitional
      if ((inputData[j][i] >= 0 && inputData[j - 1][i] >= 0) || (inputData[j][i] <= 0 && inputData[j - 1][i] <= 0))
      {
        inputDataTemp[i][j] = inputDataTemp[i][j - 1] + (inputData[j][i] - inputData[j - 1][i]);
      }
      // Transitional
      else
      {
        double integer, fraction;
        fraction = modf((inputDataTemp[i][j - 1] / 180.0), &integer);

        // High
        if ((0.5 > fraction && fraction > -0.5) || (static_cast<int>(integer) % 2))
          inputDataTemp[i][j] = inputDataTemp[i][j - 1] + (inputData[j][i] - inputData[j - 1][i]);
        // Low
        else
          // Positive
          if (inputDataTemp[i][j - 1] > 0)
            inputDataTemp[i][j] = inputDataTemp[i][j - 1] + (360 + (inputData[j][i] - inputData[j - 1][i]));
        	// Negative
          else
            inputDataTemp[i][j] = inputDataTemp[i][j - 1] + (-360 + (inputData[j][i] - inputData[j - 1][i]));
      }
    }
  }

  // Format base data
  std::vector<std::vector<std::vector<int> > > baseDataTemp(baseDataHeigth, std::vector<std::vector<int> >(baseDataDepth, std::vector<int>(baseDataWidth, 0)));

  for (unsigned int i = 0; i < baseDataHeigth; i++)
  {
    for (unsigned int j = 0; j < baseDataDepth; j++)
    {
      baseDataTemp[i][j][0] = baseData[i][0][j];

      for (unsigned int k = 1; k < baseDataWidth; k++)
      {
        // Non-transitional
        if ((baseData[i][k - 1][j] >= 0 && baseData[i][k][j] >= 0) || (baseData[i][k - 1][j] <= 0 && baseData[i][k][j] <= 0))
        {
          baseDataTemp[i][j][k] = baseDataTemp[i][j][k - 1] + (baseData[i][k][j] - baseData[i][k - 1][j]);
        }
        // Transitional
        else
        {
          double integer, fraction;
          fraction = modf((baseDataTemp[i][j][k - 1] / 180.0), &integer);

          // High
          if ((0.5 > fraction && fraction > -0.5) || (static_cast<int>(integer) % 2))
            baseDataTemp[i][j][k] = baseDataTemp[i][j][k - 1] + (baseData[i][k][j] - baseData[i][k - 1][j]);
          // Low
          else
            // Positive
            if (baseDataTemp[i][j][k - 1] > 0)
              baseDataTemp[i][j][k] = baseDataTemp[i][j][k - 1] + (360 + (baseData[i][k][j] - baseData[i][k - 1][j]));
          	// Negative
            else
              baseDataTemp[i][j][k] = baseDataTemp[i][j][k - 1] + (-360 + (baseData[i][k][j] - baseData[i][k - 1][j]));
        }
      }
    }
  }

  // Initialize
  correlationMaxIndex = 0;
  correlationMax = ((ed.calculate(inputDataTemp[0], baseDataTemp[0][0]) +
                     ed.calculate(inputDataTemp[1], baseDataTemp[0][1]) +
                     ed.calculate(inputDataTemp[2], baseDataTemp[0][2])) / 3);

  // Standard diviation base data
  for (unsigned int i = 0; i < baseDataHeigth; i++)
  {
    double correlation = ((ed.calculate(inputDataTemp[0], baseDataTemp[i][0]) +
                           ed.calculate(inputDataTemp[1], baseDataTemp[i][1]) +
                           ed.calculate(inputDataTemp[2], baseDataTemp[i][2])) / 3);

    if (correlation > correlationMax)
    {
      correlationMaxIndex = i;
      correlationMax = correlation;
    }
  }

  //cout << endl;
  //cout << "Input Data:" << "\t\t" << "Base Data: "<< endl;
  //for (unsigned int i = 0; i < baseDataWidth; i++)
  //  cout << inputDataTemp[0][i] << '\t' << inputDataTemp[1][i] << '\t' << inputDataTemp[2][i] << '\t'
  //       << baseDataTemp[correlationMaxIndex][0][i] << '\t' << baseDataTemp[correlationMaxIndex][1][i] << '\t' << baseDataTemp[correlationMaxIndex][2][i] << endl;
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
