#include <cmath>

#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

template <class T>
class EuclideanDistance {
	public:
	EuclideanDistance();
  T calculate(T* arr1, T* arr2, int len);

	private:
	T result;

};

template <class T>
EuclideanDistance<T> :: EuclideanDistance() : result(0) {
	// Do nothing
}

template <class T>
T EuclideanDistance<T> :: calculate(T* arr1, T* arr2, int len){
	for (int i = 0; i < len; i++) {
  	result += std::pow(static_cast<double>(arr1[i] - arr2[i]), 2);
	}
	return std::sqrt(result);
}

#endif // EUCLIDEANDISTANCE_H
