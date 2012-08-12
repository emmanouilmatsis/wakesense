#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <sqlite3.h>
#include "Entry.h"
#include "../../Database/Database.h"

/* -------- Class Declaration -------- */

class Index {
	public:
		Index();
		Index(const char* filename);
		void addEntry(const Entry& entry);
		int idEntry(const char* table, int argc, ...);
		bool isEntry(const char* value);

	private:
		Database database;

		void createTables();
};
