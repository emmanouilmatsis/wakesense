#include <cstdlib>

/* ======== ClusterYPR ======== */

template <class T>
class ClusterYPR {
	public:
		ClusterYPR();
		ClusterYPR(int id, T** data, unsigned int dataSize, ClusterYPR<T>* rightCluster, ClusterYPR<T>* leftCluster);
		ClusterYPR(const ClusterYPR<T>& object);
		~ClusterYPR();
		void operator =(const ClusterYPR& object);

	private:
		int id; // TODO static
		T** data;
		unsigned int dataSize;
		ClusterYPR* rightClusterYPR;
		ClusterYPR* leftClusterYPR;

		void allocateData();
		void deallocateData();
		void copyData(T** data);
		void averageData();
};

/* -------- Public -------- */

template <class T>
ClusterYPR<T> :: ClusterYPR() : id(0), data(NULL), dataSize(0), rightClusterYPR(NULL), leftClusterYPR(NULL) {
}

template <class T>
ClusterYPR<T> :: ClusterYPR(int id, T** data, unsigned int dataSize, ClusterYPR* rightClusterYPR, ClusterYPR* leftClusterYPR) : id(id), data(NULL), dataSize(dataSize), rightClusterYPR(rightClusterYPR), leftClusterYPR(leftClusterYPR) {
	allocateData();
	if ((rightClusterYPR != NULL) && (leftClusterYPR != NULL)) {
		averageData();
	} else {
		copyData(data);
	}
}

template <class T>
ClusterYPR<T> :: ClusterYPR(const ClusterYPR& object) : id(object.id), data(NULL), dataSize(object.dataSize), rightClusterYPR(object.rightClusterYPR), leftClusterYPR(object.leftClusterYPR) {
	allocateData();
	copyData(object.data);
}

template <class T>
ClusterYPR<T> :: ~ClusterYPR() {
	deallocateData();
	if (rightClusterYPR != NULL)
		delete rightClusterYPR;
	if (leftClusterYPR != NULL)
		delete leftClusterYPR;
}

template <class T>
void ClusterYPR<T> :: operator =(const ClusterYPR& object) {
	id = object.id;
	if (dataSize != object.dataSize) {
		deallocateData();
		dataSize = object.dataSize;
		allocateData();
	}
	copyData(object.data);
	rightClusterYPR = object.rightClusterYPR;
	leftClusterYPR = object.leftClusterYPR;
}

/* -------- Protected -------- */

template <class T>
void ClusterYPR<T> :: allocateData() {
	if ((data == NULL) && (dataSize != 0)) {
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
		data = NULL;
	}
}

template <class T>
void ClusterYPR<T> :: copyData(T** data) {
	if (this->data != NULL) {
		for (unsigned int i = 0; i < dataSize; i++)
			for (unsigned int j = 0; j < 3; j++)
				this->data[i][j] = data[i][j];
	}
}

template <class T>
void ClusterYPR<T> :: averageData() {
	if (dataSize != rightClusterYPR->dataSize) {
		deallocateData();
		dataSize = rightClusterYPR->dataSize;
		allocateData();
	}
	for (unsigned int i = 0; i < dataSize; i++)
		for (unsigned int j = 0; j < 3; j++) 
			data[i][j] = (rightClusterYPR->data[i][j] + leftClusterYPR->data[i][j]) / 2;
}
