#ifndef TAMIMOTOCOEFFICIENT_H
#define TAMIMOTOCOEFFICIENT_H

#include <cmath>

template <class T>
class TanimotoCoefficient {
	public:
		TamimotoCoefficient();
		double calculate(T* arr1, T* arr2, int len1, int len2);
};

template <class T>
TanimotoCoefficient <T> :: TanimotoCoefficient() {
	// Do nothing
}

double TanimotoCoefficient <T> :: calculate(T* arr1, T* arr2, int len1, int len2) {
	T arr3[len];
	int len3 = 0;
	for (int i = 0; i < len1; i++) {
		for ( int j = 0; j < len2; j++) {
			if (arr1[i] == arr2[j]) {
				arr3[i] = arr1[i];
				len3++;
			}
		}
	}
	return static_cast<double>(len3) / (len1 + len2 - len3); 
}

#endif // TAMIMOTOCOEFFICIENT_H
