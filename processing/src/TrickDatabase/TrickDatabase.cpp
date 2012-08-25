#include "TrickDatabase.h"

/* -------- Public -------- */

TrickDatabase :: TrickDatabase(std::string filename)
{
  // Database exists
  bool initialize = true;
  if (boost::filesystem::exists(filename))
    initialize = false;

  // Open database
  if (sqlite3_open(filename.c_str(), &database))
  {
    std::cerr << "Can't open database: " << sqlite3_errmsg(database) << std::endl;
    sqlite3_close(database);
    exit(1);
  }

  // Initialize database
  if (initialize)
    initializeDatabase();
}

/* ---------------- */

TrickDatabase :: ~TrickDatabase()
{
  // Close database
  sqlite3_close(database);
}

/* ---------------- */

Trick TrickDatabase :: get(int rowid)
{
  Trick trick;

  std::string statement =
    "SELECT trick.*, sample.*, location.location "
    "FROM sample "
    "JOIN location "
    "ON sample.rowid = location.sampleRowid "
    "JOIN trick "
    "ON location.trickRowid = trick.rowid "
    "AND trick.rowid="
    + ntos(rowid);

  std::vector<std::vector<std::string> > result = queryDatabase(statement);

  if (result.size() == 0)
    return trick;

  // Trick name
  trick.name = result[0][0];

  // Trick type
  trick.type = result[0][1];

  // Trick division
  trick.division = result[0][2];

  // Trick difficulty
  trick.difficulty = ston<double>(result[0][3]);

  // Trick yawMax
  trick.yawMax = ston<int>(result[0][4]);

  // Trick pitchMax
  trick.pitchMax = ston<int>(result[0][5]);

  // Trick rollMax
  trick.rollMax = ston<int>(result[0][6]);

  // Trick yaw
  // Trick pitch
  // Trick roll
  trick.yaw.resize(result.size());
  trick.pitch.resize(result.size());
  trick.roll.resize(result.size());

  for (unsigned int i = 0; i < result.size(); i++)
  {
    int location = ston<int>(result[i][10]);

    trick.yaw[location] = ston<int>(result[i][7]);
    trick.pitch[location] = ston<int>(result[i][8]);
    trick.roll[location] = ston<int>(result[i][9]);
  }

  return trick;
}

/* ---------------- */

Trick TrickDatabase :: get(std::string name)
{
  Trick trick;

  std::string statement =
    "SELECT trick.*, sample.*, location.location "
    "FROM sample "
    "JOIN location "
    "ON sample.rowid = location.sampleRowid "
    "JOIN trick "
    "ON location.trickRowid = trick.rowid "
    "AND trick.name='"
    + name +
		"'";

  std::vector<std::vector<std::string> > result = queryDatabase(statement);

  if (result.size() == 0)
    return trick;

  // Trick name
  trick.name = result[0][0];

  // Trick type
  trick.type = result[0][1];

  // Trick division
  trick.division = result[0][2];

  // Trick difficulty
  trick.difficulty = ston<double>(result[0][3]);

  // Trick yawMax
  trick.yawMax = ston<int>(result[0][4]);

  // Trick pitchMax
  trick.pitchMax = ston<int>(result[0][5]);

  // Trick rollMax
  trick.rollMax = ston<int>(result[0][6]);

  // Trick yaw
  // Trick pitch
  // Trick roll
  trick.yaw.resize(result.size());
  trick.pitch.resize(result.size());
  trick.roll.resize(result.size());

  for (unsigned int i = 0; i < result.size(); i++)
  {
    int location = ston<int>(result[i][10]);

    trick.yaw[location] = ston<int>(result[i][7]);
    trick.pitch[location] = ston<int>(result[i][8]);
    trick.roll[location] = ston<int>(result[i][9]);
  }

  return trick;
}

/* ---------------- */

void TrickDatabase :: set(Trick trick)
{
  // Trick
  int trickRowid = getTrickRowid(trick.name, trick.type, trick.division, trick.difficulty, trick.yawMax, trick.pitchMax, trick.rollMax);

  for (unsigned int i = 0; i < trick.yaw.size(); i++)
  {
    // Sample
    int sampleRowid = getSampleRowid(trick.yaw[i], trick.pitch[i], trick.roll[i]);

    // Location
    getLocationRowid(trickRowid, sampleRowid, i);
  }
}

/* ------------------------- */

void TrickDatabase :: print(std::ostream& out)
{
  std::string tableName[] = {"Trick", "Location", "Sample"};
  std::string attributeName[][8] = {{"Name", "Type", "Division", "Difficulty", "YawMax", "PitchMax", "RollMax"}, {"TrickRowid", "SampleRowid", "Location"}, {"Yaw", "Pitch", "Roll"}};
  std::string table[] = {"trick", "location", "sample"};

  for (unsigned int i = 0; i < 3; i++)
  {
    std::string statement = "SELECT * FROM " + table[i];
    std::vector<std::vector<std::string> > result = queryDatabase(statement);

    out
        << std::endl
        << "--------------------------------------------------------" << std::endl
        << "class : Index" << std::endl
        << "table : " << tableName[i] << std::endl
        << "--------------------------------------------------------" << std::endl
        << std::endl;

    out.setf(std::ios::showpos);

    for (unsigned int j = 0; j < result[0].size(); j++)
      out
          << std::setw(20) << std::left
          << attributeName[i][j];
    out << std::endl;

    for (unsigned int j = 0; j < result.size(); j++)
    {
      for (unsigned int k = 0; k < result[j].size(); k++)
        out
            << std::setw(20) << std::left
            << result[j][k];
      out << std::endl;
    }
  }
}

/* -------- Private -------- */

void TrickDatabase :: initializeDatabase()
{
  TrickGenerator trickGenerator;

  std::vector<Trick> container;

  // Novice
  container.push_back(Trick("STRAIGHT", "STRAIGHT", "NOVICE", 0.14, 0, 0, 0, trickGenerator.run(0), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("FRONTSIDE_180", "SPIN", "NOVICE", 0.18, 180, 0, 0, trickGenerator.run(180), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKSIDE_180", "SPIN", "NOVICE", 0.3, -180, 0, 0, trickGenerator.run(-180), trickGenerator.run(0), trickGenerator.run(0)));

  // Intermediate
  container.push_back(Trick("FRONTSIDE_360", "SPIN", "INTERMEDIATE", 0.44, 360, 0, 0, trickGenerator.run(360), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKSIDE_360", "SPIN", "INTERMEDIATE", 0.52, -360, 0, 0, trickGenerator.run(-360), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKROLL", "BASE_INVERT", "INTERMEDIATE", 0.42, 0, 360, 0, trickGenerator.run(0), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("FRONTROLL", "BASE_INVERT", "INTERMEDIATE", 0.44, 0, -360, 0, trickGenerator.run(0), trickGenerator.run(-360), trickGenerator.run(0)));
  container.push_back(Trick("SCARECROW", "180_INVERT", "INTERMEDIATE", 0.46, 180, -360, 0, trickGenerator.run(180), trickGenerator.run(-360), trickGenerator.run(0)));
  container.push_back(Trick("TANTRUM", "BASE_INVERT", "INTERMEDIATE", 0.44, 0, 0, 360, trickGenerator.run(0), trickGenerator.run(0), trickGenerator.run(360)));

  // Advanced
  container.push_back(Trick("BACKROLL_TO_REVERT", "180_INVERT", "ADVANCED", 0.48, 180, 360, 0, trickGenerator.run(180), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("TANTRUM_TO_REVERT", "180_INVERT", "ADVANCED", 0.48, 180, 0, 360, trickGenerator.run(180), trickGenerator.run(0), trickGenerator.run(360)));
  container.push_back(Trick("RALEY", "RALEY", "ADVANCED", 0.5, 0, 0, -90, trickGenerator.run(0), trickGenerator.run(0), trickGenerator.run(-90)));

  // Expert
  container.push_back(Trick("FRONTSIDE_540", "SPIN", "EXPERT", 0.52, 540, 0, 0, trickGenerator.run(540), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKSIDE_540", "SPIN", "EXPERT", 0.6, -540, 0, 0, trickGenerator.run(-540), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("FRONT_FLIP", "BASE_INVERT", "EXPERT", 0.54, 0, 0, -360, trickGenerator.run(0), trickGenerator.run(0), trickGenerator.run(-360)));

  // Outlaw
  container.push_back(Trick("FRONTSIDE_720", "SPIN", "OUTLAW", 0.68, 720, 0, 0, trickGenerator.run(720), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKSIDE_720", "SPIN", "OUTLAW", 0.72, -720, 0, 0, trickGenerator.run(-720), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("FRONTSIDE_900", "SPIN", "OUTLAW", 0.88, 900, 0, 0, trickGenerator.run(900), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKSIDE_900", "SPIN", "OUTLAW", 0.92, -900, 0, 0, trickGenerator.run(-900), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("FRONTSIDE_1080", "SPIN", "OUTLAW", 1, 1080, 0, 0, trickGenerator.run(1080), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKSIDE_1080", "SPIN", "OUTLAW", 1, -1080, 0, 0, trickGenerator.run(-1080), trickGenerator.run(0), trickGenerator.run(0)));
  container.push_back(Trick("BACKROLL_TO_BLIND", "180_INVERT", "OUTLAW", 0.56, -180, 360, 0, trickGenerator.run(-180), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("TANTRUM_TO_BLIND", "180_INVERT", "OUTLAW", 0.58, -180, 0, 360, trickGenerator.run(-180), trickGenerator.run(0), trickGenerator.run(360)));
  container.push_back(Trick("FRONT_FLIP_TO_REVERT", "180_INVERT", "OUTLAW", 0.6, 180, 0, -360, trickGenerator.run(180), trickGenerator.run(0), trickGenerator.run(-360)));
  container.push_back(Trick("FRONT_FLIP_TO_BLIND", "180_INVERT", "OUTLAW", 0.6, -180, 0, -360, trickGenerator.run(-180), trickGenerator.run(0), trickGenerator.run(-360)));
  container.push_back(Trick("MOBIUS", "360_INVERT", "OUTLAW", 0.64, 360, 360, 0, trickGenerator.run(360), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("MOBIUS_5", "540_INVERT", "OUTLAW", 0.72, 540, 360, 0, trickGenerator.run(540), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("KGB", "360_INVERT", "OUTLAW", 0.64, -360, 360, 0, trickGenerator.run(-360), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("KGB_5", "540_INVERT", "OUTLAW", 0.72, -540, 360, 0, trickGenerator.run(-540), trickGenerator.run(360), trickGenerator.run(0)));
  container.push_back(Trick("CROW_MOBIUS", "360_INVERT", "OUTLAW", 0.64, 360, -360, 0, trickGenerator.run(360), trickGenerator.run(-360), trickGenerator.run(0)));
  container.push_back(Trick("DUMB_DUMB", "360_INVERT", "OUTLAW", 0.66, -360, -360, 0, trickGenerator.run(-360), trickGenerator.run(-360), trickGenerator.run(0)));
  container.push_back(Trick("DUMB_DUMB_5", "540_INVERT", "OUTLAW", 0.75, -540, -360, 0, trickGenerator.run(-540), trickGenerator.run(-360), trickGenerator.run(0)));
  container.push_back(Trick("NO_NAME", "360_INVERT", "OUTLAW", 0.8, 360, 0, 360, trickGenerator.run(360), trickGenerator.run(0), trickGenerator.run(360)));
  container.push_back(Trick("MOBY_DICK", "360_INVERT", "OUTLAW", 0.6, -360, 0, 360, trickGenerator.run(-360), trickGenerator.run(0), trickGenerator.run(360)));
  container.push_back(Trick("SLIM_CHANCE", "360_INVERT", "OUTLAW", 0.68, 360, 0, -360, trickGenerator.run(360), trickGenerator.run(0), trickGenerator.run(-360)));
  container.push_back(Trick("SLIM_CHANCE_5", "540_INVERT", "OUTLAW", 0.74, 540, 0, -360, trickGenerator.run(540), trickGenerator.run(0), trickGenerator.run(-360)));
  container.push_back(Trick("FLAVOR_FLIP", "360_INVERT", "OUTLAW", 0.7, -360, 0, -360, trickGenerator.run(-360), trickGenerator.run(0), trickGenerator.run(-360)));

  // Create tables
  queryDatabase(
    "CREATE TABLE IF NOT EXISTS trick("
    "name TEXT,"
    "type TEXT,"
    "division TEXT,"
    "difficulty REAL,"
    "yawMax INTEGER,"
    "pitchMax INTEGER,"
    "rollMax INTEGER,"
    "UNIQUE (name))"
  );

  queryDatabase(
    "CREATE TABLE IF NOT EXISTS sample("
    "yaw INTEGER,"
    "pitch INTEGER,"
    "roll INTEGER,"
    "UNIQUE (yaw, pitch, roll))"
  );

  queryDatabase(
    "CREATE TABLE IF NOT EXISTS location("
    "trickRowid INTEGER,"
    "sampleRowid INTEGER,"
    "location INTEGER,"
    "PRIMARY KEY(trickRowid, sampleRowid, location),"
    "FOREIGN KEY(trickRowid) REFERENCES trick(rowid),"
    "FOREIGN KEY(sampleRowid) REFERENCES sample(rowid))"
  );

  // Insert to tables
  std::vector<Trick>::iterator iter;
  for (iter = container.begin(); iter != container.end(); iter++)
    set(*iter);
}

/* ---------------- */

std::vector<std::vector<std::string> > TrickDatabase :: queryDatabase(std::string statement)
{
  // Execute statement
  char** result;
  int resultRow;
  int resultColumn;
  char* errorMessage;
  
  if (sqlite3_get_table(database, statement.c_str(), &result, &resultRow, &resultColumn, &errorMessage) != SQLITE_OK)
  {
    std::cerr << "SQL error: " << errorMessage << std::endl;
    sqlite3_free(errorMessage);
    sqlite3_free_table(result);
    exit(1);
  }

  // Generate output
  std::vector<std::vector<std::string> > resultTemp(resultRow, std::vector<std::string> (resultColumn));

  for (int i = 0; i < resultRow; i++)
    for (int j = 0; j < resultColumn; j++)
      resultTemp[i][j] = result[(resultColumn * (i + 1)) + j];

  sqlite3_free_table(result);

  return resultTemp;
}

/* ---------------- */

int TrickDatabase :: getTrickRowid(std::string name, std::string type, std::string division, double difficulty, int yawMax, int pitchMax, int rollMax)
{
  int rowid;
  std::vector<std::vector<std::string> > result;
  std::string statement;

  // Select
  statement =
    "SELECT rowid "
    "FROM trick "
    "WHERE name='" + name + "' "
    "AND type='" + type + "' "
    "AND division='" + division + "' "
    "AND difficulty='" + ntos(difficulty) + "' "
    "AND yawMax='" + ntos(yawMax) + "' "
    "AND pitchMax='" + ntos(pitchMax) + "' "
    "AND rollMax='" + ntos(rollMax) + "'";

  result = queryDatabase(statement);

  // Insert
  if (result.size() == 0)
  {
    statement =
      "INSERT INTO trick "
      "(name, type, division, difficulty, yawMax, pitchMax, rollMax)"
      "VALUES ('"+
      name + "', '" +
      type + "', '" +
      division + "', '" +
      ntos(difficulty) + "', '" +
      ntos(yawMax) + "', '" +
      ntos(pitchMax) + "', '" +
      ntos(rollMax) +
      "')";

    queryDatabase(statement);

    // Rowid
    rowid = sqlite3_last_insert_rowid(database);
  }
  else
  {
    // Rowid
    rowid = ston<int>(result[0][0]);
  }

  return rowid;
}

/* ---------------- */

int TrickDatabase :: getLocationRowid(int trickRowid, int sampleRowid, int location)
{
  int rowid;
  std::vector<std::vector<std::string> > result;
  std::string statement;

  // Select
  statement =
    "SELECT rowid "
    "FROM location "
    "WHERE trickRowid='" + ntos(trickRowid) + "' "
    "AND sampleRowid='" + ntos(sampleRowid) + "' "
    "AND location='" + ntos(location) + "'";

  result = queryDatabase(statement);

  // Insert
  if (result.size() == 0)
  {
    statement =
      "INSERT INTO location "
      "(trickRowid, sampleRowid, location) "
      "VALUES ('"+
      ntos(trickRowid) + "', '" +
      ntos(sampleRowid) + "', '" +
      ntos(location) +
      "')";

    queryDatabase(statement);

    // Rowid
    rowid = sqlite3_last_insert_rowid(database);
  }
  else
  {
    // Rowid
    rowid = ston<int>(result[0][0]);
  }

  return rowid;
}

/* ---------------- */

int TrickDatabase :: getSampleRowid(int yaw, int pitch, int roll)
{
  int rowid;
  std::vector<std::vector<std::string> > result;
  std::string statement;

  // Select
  statement =
    "SELECT rowid "
    "FROM sample "
    "WHERE yaw='" + ntos(yaw) + "' "
    "AND pitch='" + ntos(pitch) + "' "
    "AND roll='" + ntos(roll) + "'";

  result = queryDatabase(statement);

  // Insert
  if (result.size() == 0)
  {
    statement =
      "INSERT INTO sample "
      "(yaw, pitch, roll) "
      "VALUES ('"+
      ntos(yaw) + "', '" +
      ntos(pitch) + "', '" +
      ntos(roll) +
      "')";

    queryDatabase(statement);

    // Rowid
    rowid = sqlite3_last_insert_rowid(database);
  }
  else
  {
    // Rowid
    rowid = ston<int>(result[0][0]);
  }

  return rowid;
}
