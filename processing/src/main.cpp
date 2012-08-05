#include <iostream>
#include "SetIdentifier/SetIdentifier.h"

using namespace std;

int main() {
	SetIdentifier* setIdentifier[2];
	setIdentifier[0] = new SetIdentifier2D;
  setIdentifier[1] = new SetIdentifier3D;

	cout << setIdentifier[0]->run() << endl;
	cout << setIdentifier[1]->run() << endl;

	delete setIdentifier[0];
	delete setIdentifier[1];

	return 0;
}
