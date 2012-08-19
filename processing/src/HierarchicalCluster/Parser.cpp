#include "Parser.h"

/* -------- Public -------- */

Parser :: Parser(const char* file) : file(file)
{
	// Set column and row size
	parseDataSize();

	// Allocate 3D dynamic array
	id = new char*[rowSize];
	data = new int**[rowSize];
	for (unsigned int i = 0; i < rowSize; i++)
	{
		id[i] = new char[32];
		data[i] = new int*[columnSize];
		for (unsigned int j = 0; j < columnSize; j++)
			data[i][j] = new int[3];
	}

	// Parse
	parseData();
}

Parser :: ~Parser()
{
	// Deallocate 3D dynamic array
	for (unsigned int i = 0; i < rowSize; i++)
	{
		for (unsigned int j = 0; j < columnSize; j++)
			delete [] data[i][j];
		delete [] id[i];
		delete [] data[i];
	}
	delete [] id;
	delete [] data;
}

char** Parser :: getId()
{
	return id;
}

int*** Parser :: getData()
{
	return data;
}

unsigned int Parser :: getColumnSize()
{
	return columnSize;
}

unsigned int Parser :: getRowSize()
{
	return rowSize;
}

/* -------- Private -------- */

void Parser :: parseDataSize()
{
	std :: ifstream in;
	in.open(file);
	if (in.fail())
		exit(1);

	columnSize = 0;
	rowSize = 0;

	char character;
	in.get(character);
	while (!in.eof())
	{
		if ((character == '\n'))
			rowSize++;
		if ((character == '\t') && (rowSize == 0))
			columnSize++;
		in.get(character);
	}

	in.close();
}

void Parser :: parseData()
{
	std :: ifstream in;
	in.open(file);
	if (in.fail())
		exit(1);

	// Parse
	int columnCount = 0;
	int rowCount = 0;
	char character;
	in.get(character);
	while(!in.eof())
	{
		// Parse trick type
		if (character == '#')
			in >> id[rowCount];

		// Parse trick data
		if (character == '\t')
		{
			in >> data[rowCount][columnCount][0];
			in >> data[rowCount][columnCount][1];
			in >> data[rowCount][columnCount][2];
			columnCount++; // increment column
		}
		if (character == '\n')
		{
			columnCount = 0;
			rowCount++; // increment row
		}
		// Get next character
		in.get(character);
	}
	in.close();
}
