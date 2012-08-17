#include <iostream>
#include "BTOI/BTOI.h"
#include "XBee/XBee.h"

/* -------- Namespace -------- */

using namespace std;

/* -------- Constant -------- */

#define SAMPLING_AIRBORN_DURATION 1000 // milliseconds
#define SAMPLING_DATA_INTERVAL 20 // milliseconds
#define SAMPLING_TRICK_SIZE SAMPLING_AIRBORN_DURATION / SAMPLING_DATA_INTERVAL

/* -------- Type definition -------- */

typedef unsigned char byte;
      
int main(int argc, char* argv[]) // if filename is provided then parse file, not XBee
{                           
	// Setup
	XBee xbee("/dev/tty.usbserial-A501B6XH",9600);
	XBeeRXResponse rxResponse;

	// Receive
	int trick = 0;
	int trickCount = 0;
	int sample = 0;

	while(true)
	{
		if (!(trickCount++ % 50))
		{
			trick++;
			sample = 0;
		}

		cout << "trick = " << trick << endl;
		cout << "sample = " << sample++ << endl;

		xbee.receive();
		rxResponse.setData(xbee.getResponse());

		byte payload[2];
		int payloadSize = static_cast<int>((rxResponse.getData(0x01) << 8) + (rxResponse.getData(0x02) << 0)) - 12;

		byte field = RX_RESPONSE_RECEIVE_DATA;
		for (int i = 0; i < payloadSize; i+=2)
		{
			payload[0] = rxResponse.getData(field++);
			payload[1] = rxResponse.getData(field++);
			cout <<btoi<short int>(payload) << endl;
		}
		cout << endl;
	}

	return 0;
}
