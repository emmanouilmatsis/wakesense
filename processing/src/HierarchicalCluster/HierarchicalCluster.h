#include <cstdlib>
#include "../Parser/Parser.h"
#include "../Cluster/Cluster.h"
#include "../PearsonCorrelation/PearsonCorrelation.h"

class HierarchicalCluster {
	public:
		HierarchicalCluster();
		~HierarchicalCluster();
		void run();

	private:
		double*** data;
		unsigned int columnSize;
		unsigned int rowSize;
		Cluster<double>* clusterRoot;
		Cluster<double>* cluster;
		unsigned int clusterSize;
		PearsonCorrelation <double> pearsonCorrelation;

		void initializeCluster();
		void addCluster();
		void removeCluster();
		void deleteCluster();
};
