#ifndef PEARSONCORRELATIONCOEFFICIENT_H
#define PEARSONCORRELATIONCOEFFICIENT_H

#include <cstdlib>
#include <vector>
#include <cmath>

template <class T>
class PearsonCorrelationCoefficient
{
public:
  PearsonCorrelationCoefficient();
  double calculate(std::vector<T> arr1, std::vector<T> arr2);

private:
  T sum(std::vector<T> arr);
  T sumSquare(std::vector<T> arr);
  T sumProduct(std::vector<T> arr1, std::vector<T> arr2);
};

template <class T>
PearsonCorrelationCoefficient<T> :: PearsonCorrelationCoefficient()
{
  // Do nothing
}

template <class T>
double PearsonCorrelationCoefficient<T> :: calculate(std::vector<T> arr1, std::vector<T> arr2)
{
  // Length
	double len;
	if (arr1.size() == arr2.size())
		len = static_cast<double>(arr1.size());
	else
		exit(1);

  // Sum
  T sumArr1 = sum(arr1);
  T sumArr2 = sum(arr2);

  // Square sum
  T sumSqArr1 = sumSquare(arr1);
  T sumSqArr2 = sumSquare(arr2);

  // Product sum
  T sumPr = sumProduct(arr1, arr2);

  // Correlation coefficient
  double num = sumPr - (sumArr1 * sumArr2 / len);
  double den = std::sqrt(( sumSqArr1 - (sumArr1 * sumArr1) / len) * (sumSqArr2 - (sumArr2 * sumArr2) / len));

  // Return
  if(den == 0)
  {
    return 0;
  }
  else
  {
    return num / den;
  }
}

template <class T>
T PearsonCorrelationCoefficient<T> :: sum(std::vector<T> arr)
{
  T result = 0;
  for(unsigned int i = 0; i < arr.size(); i++)
  {
    result += arr[i];
  }
  return result;
}

template <class T>
T PearsonCorrelationCoefficient<T> :: sumSquare(std::vector<T> arr)
{
  T result = 0;
  for(unsigned int i = 0; i < arr.size(); i++)
  {
    result += arr[i] * arr[i];
  }
  return result;
}

template <class T>
T PearsonCorrelationCoefficient<T> :: sumProduct(std::vector<T> arr1, std::vector<T> arr2)
{
  T result = 0;
  for(unsigned int i = 0; i < arr1.size(); i++)
  {
    result += arr1[i] * arr2[i];
  }
  return result;
}

#endif // PEARSONCORRELATIONCOEFFICIENT_H 
