#include <fstream>
#include <cstdlib>

class Parser {
	public:
		Parser(const char* file);
		~Parser();
		double*** getData();
		unsigned int getColumnSize();
		unsigned int getRowSize();

	private:
		const char* file;
		double*** data;
		unsigned int columnSize;
		unsigned int rowSize;
		void parseDataSize();
		void parseData();
};
