#include "HierarchicalCluster.h"

/* -------- Public -------- */

HierarchicalCluster :: HierarchicalCluster() : data(NULL), columnSize(0), rowSize(0), clusterRoot(NULL), cluster(NULL), clusterSize(0) {
	// Parse data
	Parser parser("data.dat");
	double*** tempData = parser.getData();

	// Allocate data (parser)
	columnSize = parser.getColumnSize();
	rowSize = parser.getRowSize();
	data = new double**[rowSize];
	for (unsigned int i = 0; i < rowSize; i++) {
		data[i] = new double*[columnSize];
		for (unsigned int j = 0; j < columnSize; j++) {
			data[i][j] = new double[3];
			for (unsigned int k = 0; k < 3; k++) { 
				data[i][j][k] = tempData[i][j][k]; // copy data
			}
		}
	}
}

HierarchicalCluster :: ~HierarchicalCluster() {
	// Deallocate data
	for (unsigned int i = 0; i < rowSize; i++) {
		for (unsigned int j = 0; j < columnSize; j++)
			delete [] data[i][j];
		delete [] data[i];
	}
	delete [] data;

	// Deallocate cluster
	if (clusterRoot != NULL)
		delete clusterRoot;
}

void HierarchicalCluster :: run() {
	// Delete cluster
	if (clusterRoot != NULL)
		delete clusterRoot;

	// ...

}

/* -------- Private -------- */

void HierarchicalCluster :: initializeCluster(Cluster* array, unsigned int& size) {
	// Initialize leaf clusters to a tricks
	size = rowSize;
	array = new Cluster[rowSize];
	for (unsigned int i = 0; i < rowSize; i++) {
		array[i].id = i;
		array[i].data = data[i];
	}	
}

void HierarchicalCluster :: deleteCluster(Cluster* array) {
	delete[] array;
	array = NULL;
}

void HierarchicalCluster :: addCluster(Cluster* element, Cluster* array, unsigned int size) {
}

void HierarchicalCluster :: removeCluster(Cluster* element, Cluster* array, unsigned int size) {
}
