#include <vector>
#include <cmath>

template <class T>
double standardDiviation(std::vector<T> input)
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
