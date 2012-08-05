#ifndef CLUSTERYPR_H
#define CLUSTERYPR_H

#include <cstdlib>

/* ======== ClusterYPR ======== */

template <class T>
class ClusterYPR {
	public:
		ClusterYPR(); // undefined cluster
		ClusterYPR(int id, T** data, unsigned int dataSize); // leaf cluster
		ClusterYPR(int id, ClusterYPR<T>* rightCluster, ClusterYPR<T>* leftCluster); // internal cluster
		~ClusterYPR();
		bool operator ==(const ClusterYPR<T>& object);

		int getId();
		T** getData();
		unsigned int getDataSize();
		ClusterYPR<T>* getRightCluster();
		ClusterYPR<T>* getLeftCluster();

	private:
		int id; // TODO static
		T** data;
		unsigned int dataSize;
		ClusterYPR<T>* rightCluster;
		ClusterYPR<T>* leftCluster;

		void allocateData();
		void deallocateData();
		void copyData(T** data);
		void averageData();
};

/* -------- Public -------- */

template <class T>
ClusterYPR<T> :: ClusterYPR() : id(0), data(NULL), dataSize(0), rightCluster(NULL), leftCluster(NULL) {
}

template <class T>
ClusterYPR<T> :: ClusterYPR(int id, T** data, unsigned int dataSize) : id(id), data(NULL), dataSize(dataSize), rightCluster(NULL), leftCluster(NULL) {
	// Copy data
	copyData(data);
}

template <class T>
ClusterYPR<T> :: ClusterYPR(int id, ClusterYPR<T>* rightCluster, ClusterYPR<T>* leftCluster) : id(id), data(NULL), dataSize(0), rightCluster(rightCluster), leftCluster(leftCluster) {
	// Avarage data
	averageData();
}

template <class T>
ClusterYPR<T> :: ~ClusterYPR() {
	// Deallocate data
	deallocateData();
}

template <class T>
bool ClusterYPR<T> :: operator ==(const ClusterYPR<T>& object) {
	return (id == object.id);
}

template <class T>
int ClusterYPR<T> :: getId() {
	return id;
}

template <class T>
T** ClusterYPR<T> :: getData() {
	return data;
}

template <class T>
unsigned int ClusterYPR<T> :: getDataSize() {
	return dataSize;
}

template <class T>
ClusterYPR<T>* ClusterYPR<T> :: getRightCluster() {
	return rightCluster;
}

template <class T>
ClusterYPR<T>* ClusterYPR<T> :: getLeftCluster() {
	return leftCluster;
}

/* -------- Private -------- */
template <class T>
void ClusterYPR<T> :: allocateData() {
	if (dataSize != 0) {
		data = new T*[dataSize];
		for (unsigned int i = 0; i < dataSize; i++)
			data[i] = new T[3];
	}
}

template <class T>
void ClusterYPR<T> :: deallocateData() {
	if (data != NULL) {
		for (unsigned int i = 0; i < dataSize; i++)
			delete[] data[i];
		delete[] data;
	}
}

template <class T>
void ClusterYPR<T> :: copyData(T** data) {
	allocateData();

	// Copy data
	if (this->data != NULL) {
		for (unsigned int i = 0; i < dataSize; i++)
			for (unsigned int j = 0; j < 3; j++)
				this->data[i][j] = data[i][j];
	}
}

template <class T>
void ClusterYPR<T> :: averageData() {
	if ((rightCluster != NULL) && (leftCluster != NULL)) {
		// dataSize = leftCluster->dataSize;
		dataSize = rightCluster->dataSize;
		allocateData();

		// Average data
		if (this->data != NULL) {
			for (unsigned int i = 0; i < dataSize; i++) 
				for (unsigned int j = 0; j < 3; j++)
					data[i][j] = (rightCluster->data[i][j] + leftCluster->data[i][j]) / 2;
		}
	}
}

#define // CLUSTERYPR_H
