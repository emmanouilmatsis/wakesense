#include <cstdlib>

/* ======== Cluster ======== */

template <class T>
class Cluster {
	public:
		Cluster();
		Cluster(int id, T* data, unsigned int dataSize, Cluster<T>* rightCluster, Cluster<T>* leftCluster);
		Cluster(const Cluster<T>& object);
		~Cluster();
		void operator =(const Cluster<T>& object);

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
Cluster<T> :: Cluster(int id, T* data, unsigned int dataSize, Cluster<T>* rightCluster, Cluster<T>* leftCluster) : id(id), data(NULL), dataSize(dataSize), rightCluster(rightCluster), leftCluster(leftCluster) {
	allocateData();
	if ((rightCluster != NULL) && (leftCluster != NULL)) {
		averageData();
	} else {
		copyData(data);
	}
}

template <class T>
Cluster<T> :: Cluster(const Cluster<T>& object) : id(object.id), data(NULL), dataSize(object.dataSize), rightCluster(object.rightCluster), leftCluster(object.leftCluster) {
	allocateData();
	copyData(object.data);
}

template <class T>
Cluster<T> :: ~Cluster() {
	deallocateData();
	if (rightCluster != NULL)
		delete rightCluster;
	if (leftCluster != NULL)
		delete leftCluster;
}

template <class T>
void Cluster<T> :: operator =(const Cluster<T>& object) {
	id = object.id;
	if (dataSize != object.dataSize) {
		deallocateData();
		dataSize = object.dataSize;
		allocateData();
	}
	copyData(object.data);
	rightCluster = object.rightCluster;
	leftCluster = object.leftCluster;
}

/* -------- Protected -------- */

template <class T>
void Cluster<T> :: allocateData() {
	if ((data == NULL) && (dataSize != 0)) {
		data = new T[dataSize];
	}
}

template <class T>
void Cluster<T> :: deallocateData() {
	if (data != NULL) {
		delete[] data;
		data = NULL;
	}
}

template <class T>
void Cluster<T> :: copyData(T* data) {
	if (this->data != NULL) {
		for (unsigned int i = 0; i < dataSize; i++)
			this->data[i] = data[i];
	}
}

template <class T>
void Cluster<T> :: averageData() {
	if (dataSize != rightCluster->dataSize) {
		deallocateData();
		dataSize = rightCluster->dataSize;
		allocateData();
	}
	for (unsigned int i = 0; i < dataSize; i++) 
		data[i] = (rightCluster->data[i] + leftCluster->data[i]) / 2;
}
