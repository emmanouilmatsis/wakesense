#include "Index.h"

/* -------- Public -------- */

Index :: Index() {
	// Create index tables
	database.query("CREATE TABLE IF NOT EXISTS trick(name TEXT NOT NULL, PRIMARY KEY(name))");

  database.query("CREATE TABLE IF NOT EXISTS sample(yaw REAL NOT NULL, pitch REAL NOT NULL, roll REAL NOT NULL, PRIMARY KEY(yaw, pitch, roll))");

	database.query("CREATE TABLE IF NOT EXISTS location(trickId INTEGER NOT NULL, sampleId INTEGER NOT NULL, location INTEGER NOT NULL, PRIMARY KEY(trickId, sampleId, location), FOREIGN KEY(trickId) REFERENCES trick(rowid), FOREIGN KEY(sampleId) REFERENCES sample(rowid))");
}

Index :: Index(const char* filename) : database(filename) {
	// Create index tables
	database.query("CREATE TABLE IF NOT EXISTS trick(name TEXT NOT NULL, PRIMARY KEY(name))");

  database.query("CREATE TABLE IF NOT EXISTS sample(yaw REAL NOT NULL, pitch REAL NOT NULL, roll REAL NOT NULL, PRIMARY KEY(yaw, pitch, roll))");

	database.query("CREATE TABLE IF NOT EXISTS location(trickId INTEGER NOT NULL, sampleId INTEGER NOT NULL, location INTEGER NOT NULL, PRIMARY KEY(trickId, sampleId, location), FOREIGN KEY(trickId) REFERENCES trick(rowid), FOREIGN KEY(sampleId) REFERENCES sample(rowid))");
}

/* -------- Private -------- */
