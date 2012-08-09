#include "Index.h"

/* -------- Public -------- */

Index :: Index() {
	// Create index tables
	createTables();
}

Index :: Index(const char* filename) : database(filename) {
	// Create index tables
	createTables();
}

void Index :: addEntry(const Entry& entry) {
	// TODO
	// struct Entry {};
	// itoa();

	// Pre-exising entry
	if (isEntry(entry.name))
		return;

	// Add trick
	int trickId = idEntry("trick", "name", entry.name);

	// Add sample and location
	for (int i = 0; i < entry.size; i++) {
		int sampleId = idEntry("sample", 3, "yaw", entry.yaw[i], "pitch", entry.pitch[i], "roll", entry.roll[i]);
		int locationId = idEntry("location", 3, "trickId", trickId, "sampleId", sampleId, "location", i);
	}
}

int Index :: idEntry(const char* table, const char* attribute, const char* value) {
	char* statement; 
	char** result;
	int resultRow;
	int resultColumn;
	int rowid;

	// Query entry
	statement = sqlite3_mprintf(
			"SELECT rowid "
			"FROM %q "
			"WHERE %q='%q'"
			, table, attribute, value);
	database.query(statement, &result, &resultRow, &resultColumn);
	sqlite3_free(statement);

	if (resultRow == 0) {

		// Free result
		database.queryFree(result);

		// Insert entry
		statement = sqlite3_mprintf(
				"INSERT INTO %q (%q) "
				"VALUES ('%q')"
				, table, attribute, value);
		database.query(statement, &result, &resultRow, &resultColumn);
		sqlite3_free(statement);

		// Rowid
		rowid = sqlite3_last_insert_rowid(database.getDatabase());
	} else {

		// Rowid
		rowid = atoi(result[resultColumn]);
	}

	// Free result
	database.queryFree(result);

	return rowid;
}

int Index :: idEntry(const char* table, int argc, ...) {
	// Variables
	va_list args;

	char* statementFormat;
	char* statement; 

	char** result;
	int resultRow;
	int resultColumn;
	int rowid;

	// Format query
	statementFormat = new char[256];
	strcpy(statementFormat, "SELECT rowid FROM ");
	strcat(statementFormat, table);
	strcat(statementFormat, " WHERE %q='%q'");
	for (int i = 0; i < argc - 1; i++)
		strcat(statementFormat, " AND %q='%q'");

	// Query entry
	va_start (args, argc);
	statement = sqlite3_vmprintf(statementFormat, args);
	database.query(statement, &result, &resultRow, &resultColumn);

	// Deallocate statement
	delete[] statementFormat;
	sqlite3_free(statement);
	va_end (args);

	if (resultRow == 0) {

		// Free result
		database.queryFree(result);

		// Format query
		statementFormat = new char[256];
		strcpy(statementFormat, "INSERT INTO ");
		strcat(statementFormat, table);
		strcat(statementFormat, " (%q");
		for (int i = 0; i < argc - 1; i++)
			strcat(statementFormat, ", %q");
		strcat(statementFormat, ") VALUES ('%q'");
		for (int i = 0; i < argc - 1; i++)
			strcat(statementFormat, ", '%q'");
		strcat(statementFormat, ")");

		// Insert entry
		va_start (args, argc);
		statement = sqlite3_mprintf(statementFormat, args);
		database.query(statement, &result, &resultRow, &resultColumn);

		// Deallocate statement
		delete[] statementFormat;
		sqlite3_free(statement);
		va_end (args);

		// Rowid
		rowid = sqlite3_last_insert_rowid(database.getDatabase());
	} else {

		// Rowid
		rowid = atoi(result[resultColumn]);
	}

	// Free result
	database.queryFree(result);

	return rowid;
}

bool Index :: isEntry(const char* value) {
	char* statement;
	char** result;
	int resultRow;
	int resultColumn;

	statement = sqlite3_mprintf(
			"SELECT * "
			"FROM trick JOIN location "
			"ON trick.rowid=location.trickId "
			"AND trick.name='%q'"
			, value);
	database.query(statement, &result, &resultRow, &resultColumn);
	sqlite3_free(statement);
	database.queryFree(result);

	if (resultRow != 0)
		return true;
	return false;
}

/* -------- Private -------- */

void Index :: createTables() {
	// Create index tables

	database.query(
			"CREATE TABLE IF NOT EXISTS trick("
			"name TEXT," 
			"PRIMARY KEY(name))"
			);

	database.query(
			"CREATE TABLE IF NOT EXISTS sample("
			"yaw INTEGER," 
			"pitch INTEGER," 
			"roll INTEGER," 
			"PRIMARY KEY(yaw, pitch, roll))"
			);

	database.query(
			"CREATE TABLE IF NOT EXISTS location("
			"trickId INTEGER," 
			"sampleId INTEGER," 
			"location INTEGER," 
			"PRIMARY KEY(trickId, sampleId, location)," 
			"FOREIGN KEY(trickId) REFERENCES trick(rowid)," 
			"FOREIGN KEY(sampleId) REFERENCES sample(rowid))"
			);
}
