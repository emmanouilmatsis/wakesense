struct Cluster {
	int id;
	double** data;
  double distance;
	Cluster* rightCluster;
	Cluster* leftCluster;
};
