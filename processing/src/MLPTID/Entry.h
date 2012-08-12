#include <cstring>

struct Entry {
	char* name;
	int size;
	int* yaw;
	int* pitch;
	int* roll;

	Entry();
	Entry(const char* name, int size, const int* yaw, const int* pitch, const int* roll);
	Entry(const Entry& structure);                                                        
	~Entry();
	void operator =(const Entry& structure);
};
