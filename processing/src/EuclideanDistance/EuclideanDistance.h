#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

#include <vector>
#include <cmath>

template <class T>
class EuclideanDistance
{
public:
  EuclideanDistance();
  double calculate(std::vector<T> x, std::vector<T> y);
};

template <class T>
EuclideanDistance<T> :: EuclideanDistance()
{
  // Do nothing
}

template <class T>
double EuclideanDistance<T> :: calculate(std::vector<T> x, std::vector<T> y)
{
  double result = 0;
  for (unsigned int i = 0; i < x.size(); i++)
  {
    result +=  ((x[i] - y[i]) * (x[i] - y[i]));
  }
  return 1 / (1 + std::sqrt(result));
}

#endif // EUCLIDEANDISTANCE_H
