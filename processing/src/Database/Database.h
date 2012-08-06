#include <cstdlib>
#include <iostream>
#include <sqlite3.h>

class Database {
	public:
		Database();
		Database(const char* filename);
		~Database();
		void query(const char* statement);
		void query(const char* statement, char*** result, int* resultRow, int* resultColumn);
		void queryFree(char** result);

	private:
		sqlite3* database;
		char* errorMessage;
		int constantMessage;

		static int callback(void *NotUsed, int argc, char** argv, char** name);
};
