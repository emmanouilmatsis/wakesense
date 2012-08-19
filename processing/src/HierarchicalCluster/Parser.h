#ifndef PARSER_H
#define PARSER_H

#include <cstdlib>
#include <fstream>
#include <cstring>

class Parser {
	public:
		Parser(const char* file);
		~Parser();
		char** getId();
		int*** getData();
		unsigned int getColumnSize();
		unsigned int getRowSize();

	private:
		const char* file;
		char** id;
		int*** data;
		unsigned int columnSize;
		unsigned int rowSize;
		void parseDataSize();
		void parseData();
};

#endif // PARSER_H
