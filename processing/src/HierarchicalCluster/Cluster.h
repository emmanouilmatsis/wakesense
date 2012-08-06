#ifndef CLUSTER_H
#define CLUSTER_H

#include <cstdlib>

/* ======== Cluster ======== */

template <class T>
class Cluster {
	public:
		Cluster(); // undefined cluster
		Cluster(int id, T* data, unsigned int dataSize); // leaf cluster
		Cluster(int id, Cluster<T>* rightCluster, Cluster<T>* leftCluster); // internal cluster
		~Cluster();
		bool operator ==(const Cluster<T>& object);

		int getId();
		T* getData();
		unsigned int getDataSize();
		Cluster<T>* getRightCluster();
		Cluster<T>* getLeftCluster();

	private:
		int id; // TODO static
		T* data;
		unsigned int dataSize;
		Cluster<T>* rightCluster;
		Cluster<T>* leftCluster;

		void allocateData();
		void deallocateData();
		void copyData(T* data);
		void averageData();
};

/* -------- Public -------- */

template <class T>
Cluster<T> :: Cluster() : id(0), data(NULL), dataSize(0), rightCluster(NULL), leftCluster(NULL) {
}

template <class T>
Cluster<T> :: Cluster(int id, T* data, unsigned int dataSize) : id(id), data(NULL), dataSize(dataSize), rightCluster(NULL), leftCluster(NULL) {
	// Copy data
	copyData(data);
}

template <class T>
Cluster<T> :: Cluster(int id, Cluster<T>* rightCluster, Cluster<T>* leftCluster) : id(id), data(NULL), dataSize(0), rightCluster(rightCluster), leftCluster(leftCluster) {
	// Avarage data
	averageData();
}

template <class T>
Cluster<T> :: ~Cluster() {
	// Deallocate data
	deallocateData();
}

template <class T>
bool Cluster<T> :: operator ==(const Cluster<T>& object) {
	return (id == object.id);
}

template <class T>
int Cluster<T> :: getId() {
	return id;
}

template <class T>
T* Cluster<T> :: getData() {
	return data;
}

template <class T>
unsigned int Cluster<T> :: getDataSize() {
	return dataSize;
}

template <class T>
Cluster<T>* Cluster<T> :: getRightCluster() {
	return rightCluster;
}

template <class T>
Cluster<T>* Cluster<T> :: getLeftCluster() {
	return leftCluster;
}

/* -------- Private -------- */
template <class T>
void Cluster<T> :: allocateData() {
	if (dataSize != 0)
		data = new T[dataSize];
}

template <class T>
void Cluster<T> :: deallocateData() {
	if (data != NULL)
		delete[] data;
}

template <class T>
void Cluster<T> :: copyData(T* data) {
	allocateData();

	// Copy data
	if (this->data != NULL) {
		for (unsigned int i = 0; i < dataSize; i++)
			this->data[i] = data[i];
	}
}

template <class T>
void Cluster<T> :: averageData() {
	if ((rightCluster != NULL) && (leftCluster != NULL)) {
		// dataSize = leftCluster->dataSize;
		dataSize = rightCluster->dataSize;
		allocateData();

    // Average data
		if (this->data != NULL) {
			for (unsigned int i = 0; i < dataSize; i++) 
				data[i] = (rightCluster->data[i] + leftCluster->data[i]) / 2;
		}
	}
}

#endif // CLUSTER_H
