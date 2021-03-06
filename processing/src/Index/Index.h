#ifndef INDEX_H
#define INDEX_H

#include <cstdlib>
#include <cstdio>
#include <ostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <stdarg.h>
#include <sqlite3.h>
#include "Entry.h"
#include "../Database/Database.h"

class Index
{
	public:
		Index();
		Index(const char* filename);
		Entry getEntry(const char* name, int version = 0);
		Entry getEntry(int entryId);
		void setEntry(Entry& entry);
		std::vector<int> getEntryIds(int version = 0);
		friend std::ostream& operator <<(std::ostream& out, const Index& object);

	private:
		Database database;

		void createTables();
		int getId(const char* table, int argc, ...);
};

#endif // INDEX_H
