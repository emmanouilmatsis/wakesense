#include "Index.h"

/* -------- Public -------- */

Index :: Index()
{
	// Create index tables
	createTables();
}

Index :: Index(const char* filename) : database(filename)
{
	// Create index tables
	createTables();
}

void Index :: addEntry(const Entry& entry)
{
	// Pre-exising entry
	if (isEntry(entry.name))
		return;

	// Add trick
	int trickIdTemp = idEntry("trick", 1, "name", entry.name);

	// Add sample and location
	for (int i = 0; i < entry.size; i++)
	{
		char yaw[8];
		char pitch[8];
		char roll[8];
		sprintf(yaw, "%d", entry.yaw[i]);
		sprintf(pitch, "%d", entry.pitch[i]);
		sprintf(roll, "%d", entry.roll[i]);
		int sampleIdTemp = idEntry("sample", 3, "yaw", yaw, "pitch", pitch, "roll", roll);

		char trickId[8];
		char sampleId[8];
		char location[8];
		sprintf(trickId, "%d", trickIdTemp);
		sprintf(sampleId, "%d", sampleIdTemp);
		sprintf(location, "%d", i);
		idEntry("location", 3, "trickId", trickId, "sampleId", sampleId, "location", location);
	}
}

int Index :: idEntry(const char* table, int argc, ...)
{
	// Variables
	va_list args;

	char* statementFormat;
	char* statement;

	char** result;
	int resultRow;
	int resultColumn;
	int rowid;

	// Format statement
	statementFormat = new char[256];
	strcpy(statementFormat, "SELECT rowid FROM ");
	strcat(statementFormat, table);
	strcat(statementFormat, " WHERE %q='%q'");
	for (int i = 0; i < argc - 1; i++)
		strcat(statementFormat, " AND %q='%q'");

	va_start (args, argc);
	statement = sqlite3_vmprintf(statementFormat, args);
	va_end (args);

	// Query entry
	database.query(statement, &result, &resultRow, &resultColumn);

	// Deallocate statement
	delete[] statementFormat;
	sqlite3_free(statement);

	if (resultRow == 0)
	{
		// Free result
		database.queryFree(result);

		// Format statement
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

		va_start (args, argc);
		statement = sqlite3_vmprintf(statementFormat, args);
		va_end (args);

		// Insert entry
		database.query(statement, &result, &resultRow, &resultColumn);

		// Deallocate statement
		delete[] statementFormat;
		sqlite3_free(statement);

		// Rowid
		rowid = sqlite3_last_insert_rowid(database.getDatabase());
	}
	else
	{
		// Rowid
		rowid = atoi(result[resultColumn]);
	}

	// Free result
	database.queryFree(result);

	return rowid;
}

bool Index :: isEntry(const char* value)
{
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

void Index :: createTables()
{
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
