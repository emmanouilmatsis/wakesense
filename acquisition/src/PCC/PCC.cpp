#include <iostream>
#include <math.h>

using namespace std;

float PCC(float* x, float* y, int len);

float sum(float* arr, int len);

float sumSquare(float* arr, int len);

float sumProduct(float* arr1, float* arr2, int len);

/* -------- Correlation coefficient -------- */

int main() {
	
  float set1[] = {2.0, -1.5, 0.0, 1.0, -1.0}; // original
  float set2[] = {2.0, -1.5, 0.0, 1.0, -1.0}; // equal
  float set3[] = {-1.0, 1.0, 0.0, -1.5, 2.0}; // reverse
	float set4[] = {0.0, 1.0, 1.5, 2.0, 0.0}; // positive
	float set5[] = {-2.0, 0.0, 1.0, 1.5, -1.5}; // random

	cout << PCC(set1, set2, 5) << endl;
	cout << PCC(set1, set3, 5) << endl;
	cout << PCC(set1, set4, 5) << endl;
	cout << PCC(set1, set5, 5) << endl;

	return 0;
}

float PCC(float* x, float* y, int len) {
	// Sum
	float sumX = sum(x, len);
	float sumY = sum(y, len);

	// Square sum
	float sumXSq = sumSquare(x, len);
	float sumYSq = sumSquare(y, len);

	// Product sum
	float sumPr = sumProduct(x, y, len);

	// Correlation coefficient 
	float num = sumPr - (sumX * sumY / len);
	float den = sqrt((( sumXSq - pow(sumX, 2) / len) * (sumYSq - pow(sumY, 2) / len)));

	// Return
	if(den == 0) {
		return 0;
	} else {
		return num / den;
	}
}

float sum(float* arr, int len) {
	float result = 0;
	for(int i=0; i<len; i++) {
		result += arr[i];
	}
	return result;
}

float sumSquare(float* arr, int len) {
	float result = 0;
	for(int i=0; i<len; i++) {
		result += pow(arr[i], 2);
	}
	return result;
}

float sumProduct(float* arr1, float* arr2, int len) {
	float result = 0;
	for(int i=0; i<len; i++) {
		result += arr1[i] * arr2[i];
	}
	return result;
}
