#ifndef PEARSONCORRELATION_H
#define PEARSONCORRELATION_H

#include <cmath>

template <class T>
class PearsonCorrelation {
	public:
		PearsonCorrelation();
		double calculate(T* arr1, T* arr2, int len);

	private:
		T sum(T* arr, int len);
		T sumSquare(T* arr, int len);
		T sumProduct(T* arr1, T* arr2, int len);

};

template <class T>
PearsonCorrelation<T> :: PearsonCorrelation() {
	// Do nothing
}

template <class T>
double PearsonCorrelation<T> :: calculate(T* arr1, T* arr2, int len) {
	// Sum
	T sumArr1 = sum(arr1, len);
	T sumArr2 = sum(arr2, len);

	// Square sum
	T sumSqArr1 = sumSquare(arr1, len);
	T sumSqArr2 = sumSquare(arr2, len);

	// Product sum
	T sumPr = sumProduct(arr1, arr2, len);

	// Correlation coefficient 
	double num = sumPr - (sumArr1 * sumArr2 / len);
	double den = std::sqrt(( sumSqArr1 - pow(static_cast<double>(sumArr1), 2) / len) * (sumSqArr2 - pow(static_cast<double>(sumArr2), 2) / len));

	// Return
	if(den == 0) {
		return 0;
	} else {
		return num / den;
	}
}

template <class T>
T PearsonCorrelation<T> :: sum(T* arr, int len) {
	T result = 0;
	for(int i = 0; i < len; i++) {
		result += arr[i];
	}
	return result;
}

template <class T>
T PearsonCorrelation<T> :: sumSquare(T* arr, int len) {
	T result = 0;
	for(int i = 0; i < len; i++) {
		result += std::pow(static_cast<double>(arr[i]), 2);
	}
	return result;
}

template <class T>
T PearsonCorrelation<T> :: sumProduct(T* arr1, T* arr2, int len) {
	T result = 0;
	for(int i = 0; i < len; i++) {
		result += arr1[i] * arr2[i];
	}
	return result;
}

#endif // PEARSONCORRELATION_H 
