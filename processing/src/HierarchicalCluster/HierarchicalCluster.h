#include "Cluster.h"
#include "../Parser/Parser.h"
#include "../PearsonCorrelation/PearsonCorrelation.h"

class HierarchicalCluster {
	public:
		HierarchicalCluster();
		HierarchicalCluster(HierarchicalCluster& object);
		~HierarchicalCluster();
		void operator =(const HierarchicalCluster& object);
		void run();

	private:
		Cluster* cluster;
		PearsonCorrelation <double> pearsonCorrelation;
};
