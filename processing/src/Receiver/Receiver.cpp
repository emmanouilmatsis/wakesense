#include "Receiver.h"

/* -------- Public -------- */

Receiver :: Receiver(const char* port, int baud, int interval, int duration)
  : xbee(port, baud), rxResponse(), data(NULL), columnSize(0), rowSize(0), ready(false), interval(interval), duration(duration)
{
}
Receiver :: ~Receiver()
{
  deallocateData();
}

int** Receiver :: getData()
{
  receive();

  return data;
}

void Receiver :: getData(const char* filename, const char* id)
{
  receive();

	std :: ofstream out;
	out.open(filename, std :: ios :: app);
	if (out.fail()) 
		exit(1);

	out << '#' << id;
	for (unsigned int i = 0; i < rowSize; i++)
	{
		out << '\t';
		for (unsigned int j = 0; j < columnSize; j++)
		{
			out << data[i][j];
			if (j < columnSize - 1)
				out << ' ';
		}
	}
	out << '\n';

	out.close();
}

unsigned int Receiver :: getColumnSize()
{
  return columnSize;
}

unsigned int Receiver :: getRowSize()
{
  return rowSize;
}

bool Receiver :: isReady()
{
  if (ready)
  {
    ready = false;
    return true;
  }

  return false;
}

/* -------- Private -------- */

void Receiver :: receive()
{
  for (unsigned int i = 0; i < duration / interval; i++)
  {
		// Receive frame
    xbee.receive();
    rxResponse.setData(xbee.getResponse());

		// Get payload size
    byte payload[2];
    columnSize = static_cast<int>((rxResponse.getData(0x01) << 8) + (rxResponse.getData(0x02) << 0)) - RX_RESPONSE_NON_PAYLOAD;

		rowSize = duration / interval;

    // Deallocate previews data
    deallocateData();

    // Allocate new data
    allocateData();

		// Get data
    byte field = RX_RESPONSE_RECEIVE_DATA;
    for (unsigned int j = 0; j < columnSize; j+=2)
    {
      payload[0] = rxResponse.getData(field++);
      payload[1] = rxResponse.getData(field++);

      data[i][j] = static_cast<int>(btoi<short int>(payload));
    }
  }

	// Data ready
  ready = true;
}

void Receiver :: allocateData()
{
  // Allocate 2D dynamic array
	if (data == NULL)
	{
		data = new int*[rowSize];
		for (unsigned int i = 0; i < rowSize; i++)
			data[i] = new int[columnSize];
	}
}

void Receiver :: deallocateData()
{
  // Deallocate 2D dynamic array
	if (data != NULL)
	{
		for (unsigned int i = 0; i < rowSize; i++)
			delete [] data[i];
		delete [] data;
		data = NULL;
	}
}
