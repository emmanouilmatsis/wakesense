#include <iostream>
#include "Serial/Serial.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
	try {

		Serial serial("/dev/tty.usbserial-A501B6XH", 9600);

		cout<<serial.readLine()<<endl;

	} catch(boost::system::system_error& e)
	{
		cout<<"Error: "<<e.what()<<endl;
		return 1;
	}
}

