#include "MLPTID.h"

/* -------- Public -------- */

MLPTID :: MLPTID()
{
	// Create tables
	createTables();
}

/* ---------------- */

MLPTID :: MLPTID(const char* filename) : database(filename)
{
	// Create tables
	createTables();
}

/* ---------------- */

std::vector<double> MLPTID :: queryNetwork(std::vector<int> sampleIds, std::vector<int> trickIds)
{
	setup(sampleIds, trickIds);
	result = feedForward();
	return result;
}

/* ---------------- */

void MLPTID :: trainNetwork(std::vector<int> sampleIds, std::vector<int> trickIds, int trickId)
{
	// Generate hidden
	generateHidden(sampleIds, trickIds);

	// Train
	setup(sampleIds, trickIds);
	feedForward();
	std::vector<double> targets(trickIds.size(), 0.0);
	for (unsigned int i = 0; i < trickIds.size(); i++)
		if (trickIds[i] == trickId)
			targets[i] = 1.0;
	backPropagate(targets);
	updateDatabase();
}

/* ------------------------- */

int MLPTID :: getId()
{
	int index = 0;
	double resultMax = result[0];
	
	for (unsigned int i = 0; i < result.size(); i++)
		if (resultMax < result[i])
		{
			resultMax = result[i];
			index = static_cast<int>(i);
		}
	
	return index;
}

/* ------------------------- */

double MLPTID :: getCorrelation()
{
	double resultMax = result[0];
	
	for (unsigned int i = 0; i < result.size(); i++)
		if (resultMax < result[i])
			resultMax = result[i];
	
	return resultMax;
}

/* ------------------------- */

void MLPTID :: print(std::ostream& out)
{
	char* statement;
	char** result;
	int resultRow;
	int resultColumn;
	Database database;

	const char* tableName[] = {"HiddenLeft", "Hidden", "HiddenRight"};
	const char* table[] = {"hiddenLeft", "hidden", "hiddenRight"};

	for (unsigned int i = 0; i < 3; i++)
	{
		statement = sqlite3_mprintf("SELECT * FROM %q", table[i]);
		database.query(statement, &result, &resultRow, &resultColumn);

    out.setf(std::ios::showpos);
    out
        << std::endl
        << "--------------------------------------------------------" << std::endl
        << "class : MLPTID" << std::endl
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

void MLPTID :: createTables()
{
	// Create MLPTID tables
	database.query(
			"CREATE TABLE IF NOT EXISTS hidden("
			"node TEXT,"
			"PRIMARY KEY(node))"
			);

	database.query(
			"CREATE TABLE IF NOT EXISTS hiddenLeft("
			"fromId INTEGER,"
			"toId INTEGER,"
			"strength REAL,"
			"PRIMARY KEY(fromId, toId))"
			);

	database.query(
			"CREATE TABLE IF NOT EXISTS hiddenRight("
			"fromId INTEGER,"
			"toId INTEGER,"
			"strength REAL,"
			"PRIMARY KEY(fromId, toId))"
			);
}

/* ---------------- */

void MLPTID :: setup(std::vector<int> sampleIds, std::vector<int> trickIds)
{
	// Value lists
	this->sampleIds = sampleIds;
	this->hiddenIds = getAllHiddenIds(sampleIds, trickIds);
	this->trickIds = trickIds;

	// Node outputs
	ai = std::vector<double>(sampleIds.size(), 1.0);
	ah = std::vector<double>(hiddenIds.size(), 1.0);
	ao = std::vector<double>(trickIds.size(), 1.0);

	// Weight input matrix
	wi = std::vector<std::vector<double> > (sampleIds.size(), std::vector<double> (hiddenIds.size(), 0.0));
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
			wi[i][j] = getStrength("hiddenLeft", sampleIds[i], hiddenIds[j]);

	// Weight output matrix
	wo = std::vector<std::vector<double> > (hiddenIds.size(), std::vector<double> (trickIds.size(), 0.0));
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
		for (unsigned int j = 0; j < trickIds.size(); j++)
			wo[i][j] = getStrength("hiddenRight", hiddenIds[i], trickIds[j]);
}

/* ---------------- */

std::vector<double> MLPTID :: feedForward()
{
	// Input activations
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		ai[i] = 1.0;

	// Hidden activations
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
	{
		double sum = 0.0;
		for (unsigned int j = 0; j < sampleIds.size(); j++)
			sum += ai[j] * wi[j][i];
		ah[i] = tanh(sum);
	}

	// Output activations
	for (unsigned int i = 0; i < trickIds.size(); i++)
	{
		double sum = 0.0;
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
			sum += ah[j] * wo[j][i];
		ao[i] = tanh(sum);
	}

	return ao;
}

/* ---------------- */

void MLPTID :: backPropagate(std::vector<double> targets, double N)
{
	// Calculate ouput errors
	std::vector<double> output_deltas(trickIds.size(), 0.0);
	for (unsigned int i = 0; i < trickIds.size(); i++)
	{
		double error = targets[i] - ao[i];
		output_deltas[i] = dtanh(ao[i]) * error;
	}

	// Calculate hidden layer errors
	std::vector<double> hidden_deltas(hiddenIds.size(), 0.0);
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
	{
		double error = 0.0;
		for (unsigned int j = 0; j < trickIds.size(); j++)
			error += output_deltas[j] * wo[i][j];
		hidden_deltas[i] = dtanh(ah[i]) * error;
	}

	// Update output weights
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
		for (unsigned int j = 0; j < trickIds.size(); j++)
		{
			double change = output_deltas[j] * ah[i];
			wo[i][j] += N * change;
		}

	// Update input weights
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
		{
			double change = hidden_deltas[j] * ai[i];
			wi[i][j] += N * change;
		}
}

/* ---------------- */

void MLPTID :: updateDatabase()
{
	// Update hiddenLeft strength
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
			setStrength("hiddenLeft", sampleIds[i], hiddenIds[j], wi[i][j]);

	// Update hiddenRight strength
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
		for (unsigned int j = 0; j < trickIds.size(); j++)
			setStrength("hiddenRight", hiddenIds[i], trickIds[j], wo[i][j]);
}

/* ---------------- */

std::vector<int> MLPTID :: getAllHiddenIds(std::vector<int> sampleIds, std::vector<int> trickIds)
{
	std::set<int> hiddenIdsSet;

	char* statement;
	char** result;
	int resultRow;
	int resultColumn;

	// Get nodes from samples
	for (unsigned int i = 0; i < sampleIds.size(); i++)
	{
		statement = sqlite3_mprintf(
				"SELECT toId "
				"FROM hiddenLeft "
				"WHERE fromId='%d'"
				, sampleIds[i]);

		database.query(statement, &result, &resultRow, &resultColumn);

		for (int j = 0; j < resultRow; j++)
			hiddenIdsSet.insert(atoi(result[(resultColumn * (j + 1))]));

		sqlite3_free(statement);
		database.queryFree(result);
	}

	// Get nodes from tricks
	for (unsigned int i = 0; i < trickIds.size(); i++)
	{
		statement = sqlite3_mprintf(
				"SELECT fromId "
				"FROM hiddenRight "
				"WHERE toId='%d'"
				, trickIds[i]);

		database.query(statement, &result, &resultRow, &resultColumn);

		for (int j = 0; j < resultRow; j++)
			hiddenIdsSet.insert(atoi(result[(resultColumn * (j + 1))]));

		sqlite3_free(statement);
		database.queryFree(result);
	}

	std::vector<int> hiddenIds(hiddenIdsSet.begin(), hiddenIdsSet.end());
	return hiddenIds;
}

/* ---------------- */

void MLPTID :: generateHidden(std::vector<int> sampleIds, std::vector<int> trickIds)
{
	//if (sampleIds.size() > 3) // TODO
		//return;

	/*
	// Node
	// Sorted with duplicates
	std::sort (sampleIds.begin(), sampleIds.end());

	char* node = new char[sampleIds.size() * 8];
	node[0] = '\0';
	for (int i = 0; i < sampleIds.size(); i++)
	{
	if (i < sampleIds.size() - 1)
	sprintf(node + strlen(node), "%d_", sampleIds[i]);
	else
	sprintf(node + strlen(node), "%d", sampleIds[i]);
	}
	 */

	// Node
	// Sorted without duplicates
	std::set<int> sampleIdsSet;
	std::set<int> :: const_iterator iter;

	for (unsigned int i = 0; i < sampleIds.size(); i++)
		sampleIdsSet.insert(sampleIds[i]);

	unsigned int index = 0;
	char* node = new char[sampleIdsSet.size() * 8];
	node[0] = '\0';
	for (iter = sampleIdsSet.begin(); iter != sampleIdsSet.end(); iter++)
	{
		if (index++ < (sampleIdsSet.size() - 1))
			sprintf(node + strlen(node), "%d_", *iter);
		else
			sprintf(node + strlen(node), "%d", *iter);
	}


	// Query
	char* statement;
	char** result;
	int resultRow;
	int resultColumn;

	// Check if node exists
	statement = sqlite3_mprintf(
			"SELECT rowid "
			"FROM hidden "
			"WHERE node='%q'"
			, node);

	database.query(statement, &result, &resultRow, &resultColumn);
	sqlite3_free(statement);

	// Create node if doesn't exists
	if (resultRow == 0)
	{
		database.queryFree(result);

		statement = sqlite3_mprintf(
				"INSERT INTO hidden (node) "
				"VALUES ('%q') "
				, node);

		database.query(statement, &result, &resultRow, &resultColumn);
		sqlite3_free(statement);

		int hiddenId = sqlite3_last_insert_rowid(database.getDatabase());

		for (unsigned int i = 0; i < sampleIds.size(); i++)
		{
			setStrength("hiddenLeft", sampleIds[i], hiddenId, (1.0 / sampleIds.size()));
		}

		for (unsigned int i = 0; i < trickIds.size(); i++)
		{
			setStrength("hiddenRight", hiddenId, trickIds[i], 0.1);
		}
	}

	database.queryFree(result);
	delete[] node;
}

/* ---------------- */

double MLPTID :: getStrength(const char* table, int fromId, int toId)
{
	char* statement;
	char** result;
	int resultRow;
	int resultColumn;
	double strength;

	statement = sqlite3_mprintf(
			"SELECT strength "
			"FROM %q "
			"WHERE fromId='%d' "
			"AND toId='%d'"
			, table, fromId, toId);
	database.query(statement, &result, &resultRow, &resultColumn);
	sqlite3_free(statement);

	if (resultRow == 0)
	{
		if (!strcmp(table, "hiddenLeft"))
			strength = -0.2;
		if (!strcmp(table, "hiddenRight"))
			strength = 0;
	}
	else
	{
		strength = atof(result[resultColumn]);
	}

	database.queryFree(result);

	return strength;
}

/* ---------------- */

void MLPTID :: setStrength(const char* table, int fromId, int toId, double strength)
{
	char* statement;
	char** result;
	int resultRow;
	int resultColumn;

	statement = sqlite3_mprintf(
			"SELECT rowid "
			"FROM %q "
			"WHERE fromId='%d' "
			"AND toId='%d'"
			, table, fromId, toId);

	database.query(statement, &result, &resultRow, &resultColumn);
	sqlite3_free(statement);

	if (resultRow == 0)
	{
		database.queryFree(result);

		statement = sqlite3_mprintf(
				"INSERT INTO %q "
				"(fromId, toId, strength) "
				"VALUES (%d, %d, %f) "
				, table, fromId, toId, strength);

		database.query(statement, &result, &resultRow, &resultColumn);
		sqlite3_free(statement);
	}
	else
	{
		char* rowid = new char[strlen(result[resultColumn]) + 1];
		strcpy(rowid, result[resultColumn]);

		database.queryFree(result);

		statement = sqlite3_mprintf(
				"UPDATE %q "
				"SET strength=%f "
				"WHERE rowid=%q"
				, table, strength, rowid);

		database.query(statement, &result, &resultRow, &resultColumn);

		sqlite3_free(statement);
		delete[] rowid;
	}

	database.queryFree(result);
}

/* ---------------- */

double MLPTID :: dtanh(double input)
{
	return (1.0 - (input * input));
}
