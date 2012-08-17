#ifndef RECEIVER_H
#define RECEIVER_H

#include <iostream>
#include <fstream>
#include "../BTOI/BTOI.h"
#include "../XBee/XBee.h"

/* -------- Constant -------- */

#define RX_RESPONSE_NON_PAYLOAD 12

/* -------- Type definition -------- */

typedef unsigned char byte;
      
/* -------- Class declaration -------- */

class Receiver
{
	public:
		Receiver(const char* port, int baud, int interval, int duration);
		~Receiver();
		int** getData();
		void getData(const char* filename, const char* id);
		unsigned int getColumnSize();
		unsigned int getRowSize();
		bool isReady();

	private:
		XBee xbee;
		XBeeRXResponse rxResponse;
		const char* file;
		int** data;
		unsigned int columnSize;
		unsigned int rowSize;
		bool ready;
		unsigned int interval;
		unsigned int duration;

		void receive();
		void allocateData();
		void deallocateData();
};

#endif // RECEIVER_H
