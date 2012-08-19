#ifndef PARSER_H
#define PARSER_H

#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

class Parser {
	public:
		Parser(std::string filename);
		std::vector<std::string> getId();
		std::vector<std::vector<std::vector<int> > > getData();

	private:
		std::string filename;
		std::vector<std::string> id;
		std::vector<std::vector<std::vector<int> > > data;
		unsigned int heigth;
		unsigned int width; 
		unsigned int depth;
		void parseDataSize();
		void parseData();
};

#endif // PARSER_H
