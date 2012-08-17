#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <cstdlib>

class Parser {
	public:
		Parser(const char* file);
		~Parser();
		int*** getData();
		unsigned int getColumnSize();
		unsigned int getRowSize();

	private:
		const char* file;
		int*** data;
		unsigned int columnSize;
		unsigned int rowSize;
		void parseDataSize();
		void parseData();
};

#endif // PARSER_H
