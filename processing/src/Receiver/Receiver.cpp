#include "Receiver.h"

/* -------- Public -------- */

Receiver :: Receiver(const char* port, int baud, int interval, int duration)
  : xbee(port, baud), rxResponse(), interval(interval), duration(duration)
{
}

std::vector<std::vector<int> > Receiver :: getData()
{
  // Receive data
  receive();

  // Export data
  return data;
}

void Receiver :: getData(const char* filename, const char* id)
{
  // Receive data
  receive();

  // Export data
  std :: ofstream out;
  out.open(filename, std::ios::app);
  if (out.fail())
    exit(1);

  if (id)
  {
    out << id;
  }
  else
  {
    char id[64];
    std::cout << "Enter id: ";
    std::cin >> id;
    out << id;
  }

  for (unsigned int i = 0; i < data.size(); i++)
  {
    out << '\t';
    for (unsigned int j = 0; j < data[0].size(); j++)
    {
      out << data[i][j];
      if (j < data[0].size() - 1)
        out << ' ';
    }
  }
  out << '\n';

  out.close();
}

void Receiver :: print(std::ostream& out)
{
  out.setf(std::ios::showpos);
  out
      << std::endl
      << "--------------------------------------------------------" << std::endl
      << "class : Receiver" << std::endl
      << "field : Id, Yaw, Pitch, Roll" << std::endl
      << "--------------------------------------------------------" << std::endl
      << std::endl;
  for (unsigned int i = 0; i < data.size(); i++)
  {
    out
        << std::setw(2) << std::left
        << i
        << std::setw(5) << std::left
        << " = {"
        << std::setw(5) << std::left
        << data[i][0]
        << std::setw(5) << std::left
        << data[i][1]
        << std::setw(5) << std::left
        << data[i][2]
        << std::setw(10) << std::left
        << '}';

    if (!((i + 1) % 5))
      out << std::endl;
  }
  out << std::endl;
}

/* -------- Private -------- */

void Receiver :: receive()
{
  data = std::vector<std::vector<int> > (duration / interval, std::vector<int>());

  for (unsigned int i = 0; i < duration / interval; i++)
  {
    // Receive frame
    xbee.receive();
    rxResponse.setData(xbee.getResponse());

    // Get payload size
    unsigned int payloadSize = (static_cast<unsigned int>((rxResponse.getData(0x01) << 8) + (rxResponse.getData(0x02) << 0)) - RX_RESPONSE_NON_PAYLOAD) / sizeof(RX_RESPONSE_PAYLOAD_DATATYPE);

    // Get payload
    byte field = RX_RESPONSE_RECEIVE_DATA;

    for (unsigned int j = 0; j < payloadSize; j++)
    {
      byte payload[2];
      payload[0] = rxResponse.getData(field++);
      payload[1] = rxResponse.getData(field++);
      data[i].push_back(static_cast<int>(btoi<RX_RESPONSE_PAYLOAD_DATATYPE>(payload)));
    }
  }
}
