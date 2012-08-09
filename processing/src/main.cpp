#include <iostream>
#include <cstdlib>
#include "TrickIdentifier/MLPTID/Index.h"

using namespace std;

int main() {
	Index index;

	char value0[] = "roll"; 
	int rowid = index.idEntry("trick", "name", value0);
	char value1[16]; 
	sprintf(value1,"%d",rowid);
	index.idEntry("location", "trickId", value1);

	char value2[] = "roll";
	cout << ((index.isEntry(value2)) ? "value = true" : "value = false") << endl;;

	return 0;
}
