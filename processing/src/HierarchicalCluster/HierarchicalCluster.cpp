#include "HierarchicalCluster.h"

HierarchicalCluster :: HierarchicalCluster() {
	// Get tricks data from file
	Parser parser("data.dat");
	double*** data = parser.getData();
	unsigned int columnSize = parser.getColumnSize();
	unsigned int rowSize = parser.getRowSize();

	// Initialize leaf clusters to a trick
	cluster = new Cluster[rowSize];
	for (unsigned int i = 0; i < rowSize; i++) {
		cluster[i].id = i;
		cluster[i].data = data[i];
	}	

	

}

HierarchicalCluster :: HierarchicalCluster(HierarchicalCluster& object) {
}

HierarchicalCluster :: ~HierarchicalCluster() {
}

void HierarchicalCluster :: operator =(const HierarchicalCluster& object) { 
}

void HierarchicalCluster :: run() {
}
