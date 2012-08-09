#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <sqlite3.h>
#include "../../Database/Database.h"

/* -------- Constants -------- */
#define TRICK 0
#define SAMPLE 1
#define LOCATION 2

/* -------- Class Declaration -------- */

class Index {
	public:
		Index();
		Index(const char* filename);
		void addEntry(const char* value);
		int idEntry(const char* table, const char* attribute, const char* value);
		int idEntry(const char* table, int argc, ...);
		bool isEntry(const char* value);

	private:
		Database database;

		void createTables();
};
