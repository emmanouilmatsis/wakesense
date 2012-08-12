#include "MLP.h"

/* -------- Public -------- */

MLP :: MLP()
{
	// Create tables
	createTables();
}

/* ---------------- */

MLP :: MLP(const char* filename) : database(filename)
{
	// Create tables
	createTables();
}

/* ---------------- */

vector<double> MLP :: queryNetwork(vector<int> sampleIds, vector<int> trickIds)
{
	setup(sampleIds, trickIds);
	return feedForward();
}

/* ---------------- */

void MLP :: trainNetwork(vector<int> sampleIds, vector<int> trickIds, int trickId)
{
	// Generate hidden
	generateHidden(sampleIds, trickIds);

	// Train
	setup(sampleIds, trickIds);
	feedForward();
	vector<double> targets(trickIds.size(), 0.0);
	for (unsigned int i = 0; i < trickIds.size(); i++) 
		if (trickIds[i] == trickId)
			targets[i] = 1.0;
	backPropagate(targets);
	update();
}

/* -------- Private -------- */

void MLP :: createTables()
{
	// Create MLP tables
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

void MLP :: setup(vector<int> sampleIds, vector<int> trickIds)
{
	// Value lists
	this->sampleIds = sampleIds;
	this->hiddenIds = getAllHiddenIds(sampleIds, trickIds);
	this->trickIds = trickIds;

	// Node outputs
	ai = vector<double>(sampleIds.size(), 1.0);
	ah = vector<double>(hiddenIds.size(), 1.0);
	ao = vector<double>(trickIds.size(), 1.0);

	// Weight matrix
  for (unsigned int i = 0; i < sampleIds.size(); i++)
	{
		wi.push_back(vector<double>());
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
			wi[i].push_back(getStrength("hiddenLeft", sampleIds[i], hiddenIds[j]));
	}

  for (unsigned int i = 0; i < hiddenIds.size(); i++)
	{
		wo.push_back(vector<double>());
		for (unsigned int j = 0; j < trickIds.size(); j++)
			wo[i].push_back(getStrength("hiddenRight", hiddenIds[i], trickIds[j]));
	}
}

/* ---------------- */

vector<double> MLP :: feedForward()
{
	// Input activations
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		ai[i] = 1.0;

	// Hidden activations
	for (unsigned int i = 0; i < hiddenIds.size(); i++)
	{
		double sum = 0.0;
		for (unsigned int j = 0; j < sampleIds.size(); j++)
		{
			sum += ai[j] * wi[j][i];
		}
		ah[i] = tanh(sum);
	}

	// Output activations
	for (unsigned int i = 0; i < trickIds.size(); i++)
	{
		double sum = 0.0;
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
		{
			sum += ah[j] * wo[j][i];                  
		}
		ao[i] = tanh(sum);
	}

	return ao;
}

/* ---------------- */

void MLP :: backPropagate(vector<double> targets, double N)
{
	// Calculate ouput errors
	vector<double> output_deltas(trickIds.size(), 0.0);
	for (unsigned int i = 0; i < trickIds.size(); i++)
	{
		double error = targets[i] - ao[i];
		output_deltas[i] = dtanh(ao[i]) * error;
	}

	// Calculate hidden layer errors
	vector<double> hidden_deltas(hiddenIds.size(), 0.0);
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

void MLP :: update()
{
	// Update database
	for (unsigned int i = 0; i < sampleIds.size(); i++)
		for (unsigned int j = 0; j < hiddenIds.size(); j++)
			setStrength("hiddenLeft", sampleIds[i], hiddenIds[j], wi[i][j]);

	for (unsigned int i = 0; i < hiddenIds.size(); i++)
		for (unsigned int j = 0; j < trickIds.size(); j++)
			setStrength("hiddenRight", hiddenIds[i], trickIds[j], wo[i][j]);
}

/* ---------------- */

vector<int> MLP :: getAllHiddenIds(vector<int> sampleIds, vector<int> trickIds)
{
	std :: set<int> hiddenIdsSet;

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
	
 	vector<int> hiddenIds(hiddenIdsSet.begin(), hiddenIdsSet.end());
	return hiddenIds;
}

/* ---------------- */

void MLP :: generateHidden(vector<int> sampleIds, vector<int> trickIds)
{
	if (sampleIds.size() > 3) // TODO
		return;

	/*
	// Node
	// Sorted with duplicates
	std :: sort (sampleIds.begin(), sampleIds.end());

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
	std :: set<int> sampleIdsSet;
	std :: set<int> :: const_iterator iter;

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
	
	cout << "node = " << node << endl; // TODO

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

double MLP :: getStrength(const char* table, int fromId, int toId)
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

void MLP :: setStrength(const char* table, int fromId, int toId, double strength)
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
		database.queryFree(result);

		statement = sqlite3_mprintf(
				"UPDATE %q "
				"SET strength=%f "
				"WHERE rowid=%d "
				, table, strength, result[resultColumn]);
		database.query(statement, &result, &resultRow, &resultColumn);
		sqlite3_free(statement);
	}

	database.queryFree(result);
}

/* ---------------- */

double MLP :: dtanh(double input)
{
	return (1.0 - (input * input));
}

// TODO delete
void MLP :: print(vector<double> input)
{
	cout << endl;
	for (unsigned int i = 0; i < input.size(); i++)
		cout << input[i] << endl;
}

void MLP :: print(vector<int> input)
{
	cout << endl;
	for (unsigned int i = 0; i < input.size(); i++)
		cout << input[i] << endl;
}
