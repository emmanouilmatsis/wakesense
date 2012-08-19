#ifndef HIERARCHICALCLUSTER_H
#define HIERARCHICALCLUSTER_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Cluster.h"
#include "Parser.h"
#include "../PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h"

#define DEBUG true

template <class T>
class HierarchicalCluster {
	public:
		HierarchicalCluster();
		~HierarchicalCluster();
		Cluster<T>* run();

	private:
		T** data;
		unsigned int columnSize;
		unsigned int rowSize;
		Cluster<T>* clusterRoot;
		PearsonCorrelationCoefficient <T> pcc;

		void addCluster(Cluster<T>* cluster, Cluster<T>**& clusterPool, unsigned int& clusterPoolSize);
		void removeCluster(int id, Cluster<T>**& clusterPool, unsigned int& clusterPoolSize);
		void deallocateHierarchicalStructure(Cluster<T>* cluster);
};

/* -------- Public -------- */

template <class T>
HierarchicalCluster<T> :: HierarchicalCluster() : data(NULL), columnSize(0), rowSize(0), clusterRoot(NULL) {
	// Parse data (3D)
	Parser parser("/Users/em/Documents/Imperial/Dissertation/implementation/processing/data/data.dat");
	T*** tempData = parser.getData();

	// Allocate data (3D -> 2D)
	columnSize = parser.getColumnSize();
	rowSize = parser.getRowSize();

	data = new T*[rowSize];
	for (unsigned int i = 0; i < rowSize; i++) {
		data[i] = new T[columnSize];
		for (unsigned int j = 0; j < columnSize; j++) {

			// Combine yaw, pitch, roll (2D)
			data[i][j] = std :: sqrt(std :: pow(tempData[i][j][0], 2) + std :: pow(tempData[i][j][1], 2) + std :: pow(tempData[i][j][2], 2));
		}
	}
}

template <class T>
HierarchicalCluster<T> :: ~HierarchicalCluster() {
	// Deallocate data (2D)
	for (unsigned int i = 0; i < rowSize; i++)
		delete [] data[i];
	delete [] data;

	// Deallocate cluster structure
	deallocateHierarchicalStructure(clusterRoot);
}

template <class T>
Cluster<T>* HierarchicalCluster<T> :: run() {
	// Deallocate cluster structure
	deallocateHierarchicalStructure(clusterRoot);

	// Allocate leaf clusters (+id)
	unsigned int clusterPoolSize = rowSize;
	Cluster<T>** clusterPool = new Cluster<T>*[clusterPoolSize];
	for (unsigned int i = 0; i < clusterPoolSize; i++)
		clusterPool[i] = new Cluster<T>(static_cast<int>(i), data[i], columnSize); 

	// Allocate internal clusters (-id)
	int clusterId = -1;
	while (clusterPoolSize > 1) {

		// Initialize
		Cluster<T>* clusterPair[2] = {clusterPool[0], clusterPool[1]};
		double distanceMax = pcc.calculate(clusterPair[0]->getData(), clusterPair[1]->getData(), columnSize);

		// Get most correlated cluster pair
		for (unsigned int i = 0; i < clusterPoolSize; i++) {
			for (unsigned int j = i + 1; j < clusterPoolSize; j++ ) {

				double distance = pcc.calculate(clusterPool[i]->getData(), clusterPool[j]->getData(), columnSize);

				if (distance > distanceMax) {
					clusterPair[0] = clusterPool[i];
					clusterPair[1] = clusterPool[j];
					distanceMax = distance;
				}
			}
		}

#if DEBUG

		std :: cout << std :: endl << "clusterPool = ";
		for (unsigned int i = 0; i < clusterPoolSize; i++)
			std :: cout << "\t" << clusterPool[i]->getId();
		std :: cout << std :: endl;
		std :: cout << "clusterNewId = " << clusterId << " clusterA = " << clusterPair[0]->getId() << " clusterB = " << clusterPair[1]->getId() << std :: endl << std :: endl;

#endif

		// Update cluster pool
		addCluster(new Cluster<T>(clusterId, clusterPair[0], clusterPair[1]), clusterPool, clusterPoolSize);
		removeCluster(clusterPair[0]->getId(), clusterPool, clusterPoolSize);
		removeCluster(clusterPair[1]->getId(), clusterPool, clusterPoolSize);
		clusterId--;
	}

	// Hierarchical cluster root
	clusterRoot = clusterPool[0];
	return clusterRoot;
}

/* -------- Private -------- */

template <class T>
void HierarchicalCluster<T> :: addCluster(Cluster<T>* cluster, Cluster<T>**& clusterPool, unsigned int& clusterPoolSize) {
	// Allocate temporary cluster pool
	Cluster<T>** clusterPoolTemp = new Cluster<T>*[clusterPoolSize + 1];

	// Fill temporary cluster pool
	for (unsigned int i = 0; i < clusterPoolSize; i++)
		clusterPoolTemp[i] = clusterPool[i]; // existing clusters
	clusterPoolTemp[clusterPoolSize] = cluster; // new cluster

	// Deallocate cluster pool
	delete[] clusterPool;

	// Update cluster pool
	clusterPool = clusterPoolTemp;
	clusterPoolSize++;
}

template <class T>
void HierarchicalCluster<T> :: removeCluster(int id, Cluster<T>**& clusterPool, unsigned int& clusterPoolSize) {
	// Allocate temporary cluster pool
	Cluster<T>** clusterPoolTemp;
	if (clusterPoolSize > 1) // non-empty temporary cluster pool
		clusterPoolTemp = new Cluster<T>*[clusterPoolSize - 1]; 
	else // empty temporary cluster pool
		clusterPoolTemp = NULL;

	// Fill temporary cluster pool
	unsigned int index = 0;
	for (unsigned int i = 0; (i < clusterPoolSize) && (index < clusterPoolSize - 1); i++)
		if (!(clusterPool[i]->getId() == id)) {
			clusterPoolTemp[index] = clusterPool[i]; // existing clusters except cluster
			index++;
		}

	// Deallocate cluster pool
	delete[] clusterPool;

	// Update cluster pool
	clusterPool = clusterPoolTemp;
	clusterPoolSize--;
}

template <class T>
void HierarchicalCluster<T> :: deallocateHierarchicalStructure(Cluster<T>* cluster) {
	// Base case
	if (cluster == NULL)
		return;

	// Recursive case
	deallocateHierarchicalStructure(cluster->getRightCluster());
	deallocateHierarchicalStructure(cluster->getLeftCluster());
	delete cluster;
}

#endif // HIERARCHICALCLUSTER_H
