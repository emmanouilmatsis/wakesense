#include <cstdlib>
#include <sqlite3.h>
#include "../../Database/Database.h"

class Index {
	public:
	Index();
	Index(const char* filename);
	void addEntry();
	void idEntry();
	bool isEntry();
	void addLink();
	void idLink();
	bool isLink();
	
	private:
	Database database;
};
