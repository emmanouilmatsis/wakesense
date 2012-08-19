#include "Index.h"

/* -------- Public -------- */

Index :: Index()
{
  // Create index tables
  createTables();
}

/* ------------------------- */

Index :: Index(const char* filename) : database(filename)
{
  // Create index tables
  createTables();
}

/* ------------------------- */

Entry Index :: getEntry(const char* name, int version)
{
  Entry entry;

  char* statement;
  char** result;
  int resultRow;
  int resultColumn;

  // Is entry
  statement = sqlite3_mprintf(
                "SELECT * "
                "FROM trick "
                "JOIN location "
                "ON trick.rowid=location.trickId "
                "AND trick.name='%q'"
                "AND trick.version=%d"
                , name, version);
  database.query(statement, &result, &resultRow, &resultColumn);
  sqlite3_free(statement);
  database.queryFree(result);

  if (resultRow == 0)
    return entry;

  // Entry name
  strcpy(entry.name, name);

  char versionTemp[8];

  // Entry trickId
  sprintf(versionTemp, "%d", 0);
  entry.trickId = getId("trick", 2, "name", name, "version", versionTemp);

  // Entry version
  entry.version = version;

  // Entry versionId
  sprintf(versionTemp, "%d", version);
  entry.versionId = getId("trick", 2, "name", name, "version", versionTemp);

  // Entry size, samples and sampleIds
  statement = sqlite3_mprintf(
                "SELECT sample.yaw, sample.pitch, sample.roll, sample.rowid, location.location "
                "FROM sample "
                "JOIN location "
                "ON sample.rowid = location.sampleId "
                "JOIN trick "
                "ON location.trickId = trick.rowId "
                "WHERE trick.name='%q'"
                "AND trick.version=%d"
                , name, version);
  database.query(statement, &result, &resultRow, &resultColumn);
  sqlite3_free(statement);

  entry.size = resultRow;
  entry.yaw.resize(resultRow);
  entry.pitch.resize(resultRow);
  entry.roll.resize(resultRow);
  entry.sampleIds.resize(resultRow);

  for (int i = 0; i < resultRow; i++)
  {
    int location = atoi(result[((i + 1) * resultColumn) + 4]);

    entry.yaw[location] = atoi(result[((i + 1) * resultColumn) + 0]);
    entry.pitch[location] = atoi(result[((i + 1) * resultColumn) + 1]);
    entry.roll[location] = atoi(result[((i + 1) * resultColumn) + 2]);
    entry.sampleIds[location] = atoi(result[((i + 1) * resultColumn) + 3]);
  }

  database.queryFree(result);

  return entry;
}

/* ------------------------- */

Entry Index :: getEntry(int entryId, int version)
{
  Entry entry;

  char* statement;
  char** result;
  int resultRow;
  int resultColumn;

  // Is entry
  statement = sqlite3_mprintf(
                "SELECT trick.name "
                "FROM trick "
                "JOIN location "
                "ON trick.rowid=location.trickId "
                "AND trick.rowid=%d"
                "AND trick.version=%d"
                , entryId, version);
  database.query(statement, &result, &resultRow, &resultColumn);
  sqlite3_free(statement);

  if (resultRow == 0)
	{
  	database.queryFree(result);
    return entry;
	}

  // Entry name
  strcpy(entry.name, result[resultColumn]);
  database.queryFree(result);

  char versionTemp[8];

  // Entry trickId
  sprintf(versionTemp, "%d", 0);
  entry.trickId = entryId;

  // Entry version
  entry.version = version;

  // Entry versionId
  sprintf(versionTemp, "%d", version);
  entry.versionId = getId("trick", 2, "name", entry.name, "version", versionTemp);

  // Entry size, samples and sampleIds
  statement = sqlite3_mprintf(
                "SELECT sample.yaw, sample.pitch, sample.roll, sample.rowid, location.location "
                "FROM sample "
                "JOIN location "
                "ON sample.rowid = location.sampleId "
                "JOIN trick "
                "ON location.trickId = trick.rowId "
                "WHERE trick.rowid=%d"
                "AND trick.version=%d"
                , entryId, version);
  database.query(statement, &result, &resultRow, &resultColumn);
  sqlite3_free(statement);

  entry.size = resultRow;
  entry.yaw.resize(resultRow);
  entry.pitch.resize(resultRow);
  entry.roll.resize(resultRow);
  entry.sampleIds.resize(resultRow);

  for (int i = 0; i < resultRow; i++)
  {
    int location = atoi(result[((i + 1) * resultColumn) + 4]);

    entry.yaw[location] = atoi(result[((i + 1) * resultColumn) + 0]);
    entry.pitch[location] = atoi(result[((i + 1) * resultColumn) + 1]);
    entry.roll[location] = atoi(result[((i + 1) * resultColumn) + 2]);
    entry.sampleIds[location] = atoi(result[((i + 1) * resultColumn) + 3]);
  }

  database.queryFree(result);

  return entry;
}

/* ------------------------- */

void Index :: setEntry(Entry& entry)
{
  // Get trick version
  char* statement;
  char** result;
  int resultRow;
  int resultColumn;

  statement = sqlite3_mprintf(
                "SELECT * "
                "FROM trick "
                "WHERE name='%q'"
                , entry.name);
  database.query(statement, &result, &resultRow, &resultColumn);
  sqlite3_free(statement);
  database.queryFree(result);

  char versionTemp[8];

  // Entry trickId
  sprintf(versionTemp, "%d", 0);
  entry.trickId = getId("trick", 2, "name", entry.name, "version", versionTemp);

  // Entry version
  entry.version = resultRow;

  // Entry versionId
  sprintf(versionTemp, "%d", entry.version);
  entry.versionId = getId("trick", 2, "name", entry.name, "version", versionTemp);

  for (unsigned int i = 0; i < entry.size; i++)
  {
    // Entry sampleIds
    char yawTemp[8];
    char pitchTemp[8];
    char rollTemp[8];
    sprintf(yawTemp, "%d", entry.yaw[i]);
    sprintf(pitchTemp, "%d", entry.pitch[i]);
    sprintf(rollTemp, "%d", entry.roll[i]);
    entry.sampleIds[i] = getId("sample", 3, "yaw", yawTemp, "pitch", pitchTemp, "roll", rollTemp);

    // Entry location
    char versionIdTemp[8];
    char sampleIdTemp[8];
    char locationTemp[8];
    sprintf(versionIdTemp, "%d", entry.versionId);
    sprintf(sampleIdTemp, "%d", entry.sampleIds[i]);
    sprintf(locationTemp, "%d", i);
    getId("location", 3, "trickId", versionIdTemp, "sampleId", sampleIdTemp, "location", locationTemp);
  }
}

/* ------------------------- */

std::vector<int> Index :: getEntryIds(int version)
{
  // Get trickIds
  char* statement;
  char** result;
  int resultRow;
  int resultColumn;

  statement = sqlite3_mprintf(
                "SELECT rowid "
                "FROM trick "
                "WHERE version=%d"
                , version);
  database.query(statement, &result, &resultRow, &resultColumn);
  sqlite3_free(statement);

  std::vector<int> entryIds(resultRow, 0);
  for (int i = 0; i < resultRow; i++)
    entryIds[i] = atoi(result[resultColumn + i]);

  database.queryFree(result);

  return entryIds;
}

/* ------------------------- */

void Index :: print(std::ostream& out)
{
  char* statement;
  char** result;
  int resultRow;
  int resultColumn;
  Database database;

  // Get nodes from samples
  const char* tableName[] = {"Sample", "Location", "Trick"};
  const char* table[] = {"sample", "location", "trick"};

  for (unsigned int i = 0; i < 3; i++)
  {
    statement = sqlite3_mprintf("SELECT * FROM %q", table[i]);
    database.query(statement, &result, &resultRow, &resultColumn);

    out.setf(std::ios::showpos);
    out
        << std::endl
        << "--------------------------------------------------------" << std::endl
        << "class : Index" << std::endl
        << "table : " << tableName[i] << std::endl
        << "--------------------------------------------------------" << std::endl
        << std::endl;
    for (int i = 0; i < ((resultRow + 1) * resultColumn); i++)
    {
      out
          << std::setw(10) << std::left
          << result[i];
      if (!((i + 1) % resultColumn))
        out << std::endl;
    }

    sqlite3_free(statement);
    database.queryFree(result);
  }
}

/* -------- Private -------- */

void Index :: createTables()
{
  // Create index tables
  database.query(
    "CREATE TABLE IF NOT EXISTS trick("
    "name TEXT,"
    "version INTEGER,"
    "UNIQUE (name, version))"
  );

  database.query(
    "CREATE TABLE IF NOT EXISTS sample("
    "yaw INTEGER,"
    "pitch INTEGER,"
    "roll INTEGER,"
    "UNIQUE (yaw, pitch, roll))"
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

/* ------------------------- */

int Index :: getId(const char* table, int argc, ...)
{
  // Variables
  va_list args;

  int rowid;

  char* statementFormat;
  char* statement;
  char** result;
  int resultRow;
  int resultColumn;

  // Format statement
  statementFormat = new char[256];
  strcpy(statementFormat, "SELECT rowid FROM ");
  strcat(statementFormat, table);
  strcat(statementFormat, " WHERE %q='%q'");
  for (int i = 1; i < argc; i++)
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
    va_start (args, argc);
    char** argsTemp = new char*[argc * 2];
    for (int i = 0; i < argc * 2; i++)
    {
      argsTemp[i] = new char[64]; // TODO argument string size
      strcpy(argsTemp[i], va_arg(args, char*));
    }
    va_end (args);

    statement = new char[256];
    int length = 0;
    length += sprintf(statement + length, "INSERT INTO %s ", table);
    length += sprintf(statement + length, "(%s", argsTemp[0]);
    for (int i = 1; i < argc; i++)
      length += sprintf(statement + length, ", %s", argsTemp[2 * i]);
    length += sprintf(statement + length, ") VALUES ('%s'", argsTemp[1]);
    for (int i = 1; i < argc; i++)
      length += sprintf(statement + length, ", '%s'", argsTemp[(2 * i) + 1]);
    length += sprintf(statement + length, ")");

    for (int i = 0; i < argc * 2; i++)
      delete[] argsTemp[i];
    delete[] argsTemp;

    // Insert entry
    database.query(statement, &result, &resultRow, &resultColumn);

    // Deallocate statement
    delete[] statement;

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
