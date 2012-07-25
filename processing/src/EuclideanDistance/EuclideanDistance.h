#include <cmath>

#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

template <class T>
class EuclideanDistance {
	public:
		EuclideanDistance();
		double calculate(T* arr1, T* arr2, int len);
};

template <class T>
EuclideanDistance<T> :: EuclideanDistance() {
	// Do nothing
}

template <class T>
double EuclideanDistance<T> :: calculate(T* arr1, T* arr2, int len){
	double result = 0;
	for (int i = 0; i < len; i++) {
		result += std::pow(static_cast<double>(arr1[i] - arr2[i]), 2);
	}
	return 1 / (1 + std::sqrt(result));
}

#endif // EUCLIDEANDISTANCE_H
