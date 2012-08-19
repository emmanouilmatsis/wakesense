#include "Parser.h"

/* -------- Public -------- */

Parser :: Parser(std::string filename) : filename(filename)
{
	// Set column and row size
	parseDataSize();

	// Initialize id
  id = std::vector<std::string>(heigth, std::string());

	// Initialize data
	data = std::vector<std::vector<std::vector<int> > >(heigth, std::vector<std::vector<int> > (width, std::vector<int> (depth, 0)));

	// Parse
	parseData();
}

std::vector<std::string> Parser :: getId()
{
	return id;
}

std::vector<std::vector<std::vector<int> > > Parser :: getData()
{
	return data;
}

/* -------- Private -------- */

void Parser :: parseDataSize()
{
	std :: ifstream in;
	in.open(filename.c_str());
	if (in.fail())
		exit(1);

	heigth = 0;
	width = 0;
	depth = 1;

	char character;
	in.get(character);
	while (!in.eof())
	{
		if ((character == '\n'))
			heigth++;
		if ((character == '\t') && (heigth == 0))
			width++;
		if ((character == ' ') && (width == 1))
			depth++;
		in.get(character);
	}

	in.close();
}

void Parser :: parseData()
{
	std :: ifstream in;
	in.open(filename.c_str());
	if (in.fail())
		exit(1);

	for (unsigned int i = 0; i < heigth; i++)
	{
		// Parse id
		in >> id[i];

		// Parse data
		for (unsigned int j = 0; j < width; j++)
			for (unsigned int k = 0; k < depth; k++)
				in >> data[i][j][k];
	}

	in.close();
}
