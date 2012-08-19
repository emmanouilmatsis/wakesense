#include "Database.h"

/* -------- Public -------- */

Database :: Database() {
	// Default filename
	const char* filename = "/Users/em/Documents/Imperial/Dissertation/implementation/processing/data/database.db";

	// Open connection
	constantMessage = sqlite3_open(filename, &database);

	if (constantMessage) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(database) << std::endl;
		sqlite3_close(database);
		exit(1);
	}
}

Database :: Database(const char* filename) {
	// Open connection
	constantMessage = sqlite3_open(filename, &database);

	if (constantMessage) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(database) << std::endl;
		sqlite3_close(database);
		exit(1);
	}
}

Database :: ~Database() {
	// Close connection
	sqlite3_close(database);
}

void Database :: query(const char* statement) {
	// Execute statement
	constantMessage = sqlite3_exec(database, statement, Database :: callback, 0, &errorMessage);
	
	if (constantMessage != SQLITE_OK) {
		std::cerr << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
	}
}

void Database :: query(const char* statement, char*** result, int* resultRow, int* resultColumn) {
	// Execute statement
	constantMessage = sqlite3_get_table(database, statement, result, resultRow, resultColumn, &errorMessage);
	
	if (constantMessage != SQLITE_OK) {
		std::cerr << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
	}
}

void Database :: queryFree(char** result) {
	sqlite3_free_table(result);
}

sqlite3* Database :: getDatabase() {
	return database;
}

/* -------- Private -------- */

int Database :: callback(void *NotUsed, int argc, char** argv, char** name) {
	// Print data
	for (int i = 0; i < argc; i++)
		std::cout << name[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
	std::cout << std::endl;
	return 0;
}
