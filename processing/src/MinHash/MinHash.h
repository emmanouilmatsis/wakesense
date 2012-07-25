#include <cmath>

#ifndef MINHASH_H
#define MINHASH_H

template <class T>
class MinHash {
	public:
		MinHash();
		double calculate(T* arr1, T* arr2, int len1, int len2);

	private:

};

template <class T>
MinHash<T> :: MinHash() {
	// Do nothing
}

template <class T>
double MinHash<T> :: calculate(T* arr1, T* arr2, int len1, int len2) {
	// ...
}

#endif // MINHASH_H
