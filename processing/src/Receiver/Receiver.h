#ifndef RECEIVER_H
#define RECEIVER_H

#include <ostream>
#include <fstream>
#include <iomanip>
#include "../Converter/Converter.h"
#include "../XBee/XBee.h"

/* -------- Constant -------- */

#define RX_RESPONSE_NON_PAYLOAD 12
#define RX_RESPONSE_PAYLOAD_DATATYPE short int

/* -------- Type definition -------- */

typedef unsigned char byte;
      
/* -------- Class declaration -------- */

class Receiver
{
	public:
		Receiver(const char* port, int baud, int interval, int duration);
		std::vector<std::vector<int> > getData();
		void getData(const char* filename, const char* id = NULL);
		void print(std::ostream& out);

	private:
		XBee xbee;
		XBeeRXResponse rxResponse;
		std::vector<std::vector<int> > data;
		unsigned int interval;
		unsigned int duration;

		void receive();
};

#endif // RECEIVER_H
