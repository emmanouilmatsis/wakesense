#include <iostream>
#include "HierarchicalCluster/HierarchicalCluster.h"

template <class T>
void printHierarchicalStructure(Cluster<T>* cluster) {
	// Base case 1
	if (cluster == NULL) {
		std :: cout << "NULL";
		return;
	}

	// Recursive case
	std :: cout << std :: endl << cluster->getId() << " ";
	printHieararchicalStructre(cluster->getRightCluster());
	printHieararchicalStructre(cluster->getLeftCluster());
}

int main() {
	HierarchicalCluster<double> hc;
	hc.run();

	return 0;
}
