#ifndef XBee_h
#define XBee_h

#include "../SerialTimeout/SerialTimeout.h"

/* -------- API ID -------- */
#define AT_COMMAND_REQUEST 0x08
#define TX_REQUEST 0x10
#define AT_COMMAND_RESPONSE 0x88
#define RX_RESPONSE 0x90
#define RX_IO_RESPONSE 0x92
#define TX_STATUS_RESPONSE 0x8B

/* -------- Frame Fields -------- */

// FRAME
#define FRAME_START_DELIMITER 0x00
#define FRAME_LENGTH_MSB 0x01
#define FRAME_LENGTH_LSB 0x02
#define FRAME_CHECKSUM 0xFF

// AT_COMMAND_REQUEST
#define AT_COMMAND_REQUEST_API_ID 0x03
#define AT_COMMAND_REQUEST_FRAME_ID 0x04
#define AT_COMMAND_REQUEST_AT_COMMAND_0 0x05
#define AT_COMMAND_REQUEST_AT_COMMAND_1 0x06
#define AT_COMMAND_REQUEST_PARAMETER_VALUE 0x07

// TX_REQUEST
#define TX_REQUEST_API_ID 0x03
#define TX_REQUEST_FRAME_ID 0x04
#define TX_REQUEST_DESTINATION_ADDRESS_0 0x05
#define TX_REQUEST_DESTINATION_ADDRESS_1 0x06
#define TX_REQUEST_DESTINATION_ADDRESS_2 0x07
#define TX_REQUEST_DESTINATION_ADDRESS_3 0x08
#define TX_REQUEST_DESTINATION_ADDRESS_4 0x09
#define TX_REQUEST_DESTINATION_ADDRESS_5 0x0A
#define TX_REQUEST_DESTINATION_ADDRESS_6 0x0B
#define TX_REQUEST_DESTINATION_ADDRESS_7 0x0C
#define TX_REQUEST_NETWORK_ADDRESS_0 0x0D
#define TX_REQUEST_NETWORK_ADDRESS_1 0x0E
#define TX_REQUEST_BROADCAST_RADIUS 0x0F
#define TX_REQUEST_OPTIONS 0x10
#define TX_REQUEST_RF_DATA 0x11

// AT_COMMAND_RESPONSE
#define AT_COMMAND_RESPONSE_API_ID 0x03
#define AT_COMMAND_RESPONSE_FRAME_ID 0x04
#define AT_COMMAND_RESPONSE_AT_COMMAND_0 0x05
#define AT_COMMAND_RESPONSE_AT_COMMAND_1 0x06
#define AT_COMMAND_RESPONSE_COMMAND_STATUS 0x07
#define AT_COMMAND_RESPONSE_COMMAND_DATA 0x08

// RX_RESPONSE
#define RX_RESPONSE_API_ID 0x03
#define RX_RESPONSE_SOURCE_ADDRESS_0 0x04
#define RX_RESPONSE_SOURCE_ADDRESS_1 0x05
#define RX_RESPONSE_SOURCE_ADDRESS_2 0x06
#define RX_RESPONSE_SOURCE_ADDRESS_3 0x07
#define RX_RESPONSE_SOURCE_ADDRESS_4 0x08
#define RX_RESPONSE_SOURCE_ADDRESS_5 0x09
#define RX_RESPONSE_SOURCE_ADDRESS_6 0x0A
#define RX_RESPONSE_SOURCE_ADDRESS_7 0x0B
#define RX_RESPONSE_NETWORK_ADDRESS_0 0x0C
#define RX_RESPONSE_NETWORK_ADDRESS_1 0x0D
#define RX_RESPONSE_RECEIVE_OPTIONS 0x0E
#define RX_RESPONSE_RECEIVE_DATA 0x0F

// RX_IO_RESPONSE
#define RX_IO_RESPONSE_API_ID 0x03
#define RX_IO_RESPONSE_SOURCE_ADDRESS_0 0x04
#define RX_IO_RESPONSE_SOURCE_ADDRESS_1 0x05
#define RX_IO_RESPONSE_SOURCE_ADDRESS_2 0x06
#define RX_IO_RESPONSE_SOURCE_ADDRESS_3 0x07
#define RX_IO_RESPONSE_SOURCE_ADDRESS_4 0x08
#define RX_IO_RESPONSE_SOURCE_ADDRESS_5 0x09
#define RX_IO_RESPONSE_SOURCE_ADDRESS_6 0x0A
#define RX_IO_RESPONSE_SOURCE_ADDRESS_7 0x0B
#define RX_IO_RESPONSE_NETWORK_ADDRESS_0 0x0C
#define RX_IO_RESPONSE_NETWORK_ADDRESS_1 0x0D
#define RX_IO_RESPONSE_RECEIVE_OPTIONS 0x0E
#define RX_IO_RESPONSE_NUMBER_OF_SAMPLES 0x0F
#define RX_IO_RESPONSE_DIGITAL_CHANNEL_MASK_0 0x10
#define RX_IO_RESPONSE_DIGITAL_CHANNEL_MASK_1 0x11
#define RX_IO_RESPONSE_ANALOG_CHANNEL_MASK 0x12
#define RX_IO_RESPONSE_DIGITAL_SAMPLES_0 0x13
#define RX_IO_RESPONSE_DIGITAL_SAMPLES_1 0x14
#define RX_IO_RESPONSE_ANALOG_SAMPLES_0 0x15
#define RX_IO_RESPONSE_ANALOG_SAMPLES_1 0x16

// TX_STATUS_RESPONSE
#define TX_STATUS_RESPONSE_API_ID 0x03
#define TX_STATUS_RESPONSE_FRAME_ID 0x04
#define TX_STATUS_RESPONSE_NETWORK_ADDRESS_0 0x05
#define TX_STATUS_RESPONSE_NETWORK_ADDRESS_1 0x06
#define TX_STATUS_RESPONSE_TRANSMIT_RETRY_COUNT 0x07
#define TX_STATUS_RESPONSE_DELIVERY_STATUS 0x08
#define TX_STATUS_RESPONSE_DISCOVERY_STATUS 0x09

/* -------- Frame Field Constants -------- */

// FRAME
#define START_DELIMITER 0x7E
#define ESCAPE 0x7D
#define ON 0x11
#define OFF 0x13

// AT_COMMAND_REQUEST_FRAME_ID
#define AT_COMMAND_REQUEST_DISABLE_ACK 0x00
#define AT_COMMAND_REQUEST_ENABLE_ACK 0x01

// TX_REQUEST_FRAME_ID
#define TX_REQUEST_DISABLE_ACK 0x00
#define TX_REQUEST_ENABLE_ACK 0x01

// TX_REQUEST_DESTINATION_ADDRESS
#define TX_REQUEST_COORDINATOR_ADDRESS 0x0000000000000000
#define TX_REQUEST_BROADCAST_ADDRESS 0x000000000000FFFF

// TX_REQUEST_DESTINATION_NETWORK_ADDRESS
#define TX_REQUEST_NETWORK_BROADCAST_ADDRESS 0xFFFE

// TX_REQUEST_BROADCAST_RADIUS
#define TX_REQUEST_MAX_HOPS 0x00

// TX_REQUEST_OPTIONS
#define TX_REQUEST_OPTION 0x00

// AT_COMMAND_RESPONSE_COMMAND_STATUS
#define AT_COMMAND_RESPONSE_OK 0x00
#define AT_COMMAND_RESPONSE_ERROR 0x01
#define AT_COMMAND_RESPONSE_INVALID_COMMAND 0x02
#define AT_COMMAND_RESPONSE_INVALID_PARAMETER 0x03

// RX_RESPONSE_SOURCE_ADDRESS
#define RX_RESPONSE_UNKNOWN_SOURCE_ADDRESS 0xFFFFFFFFFFFFFFFF

// RX_RESPONSE_RECEIVE_OPTIONS
#define RX_RESPONSE_ACK_PACKET 0x01
#define RX_RESPONSE_BROADCAST_PACKET 0x02
#define RX_RESPONSE_APS_PACKET 0x20
#define RX_RESPONSE_END_DEVICE_PACKET 0x40

// RX_IO_RESPONSE_RECEIVE_OPTIONS
#define RX_IO_RESPONSE_ACK_PACKET 0x01
#define RX_IO_RESPONSE_BROADCAST_PACKET 0x02

// TX_STATUS_RESONSE_DELIVERY_STATUS
#define TX_STATUS_RESPONSE_SUCCESS 0x00
#define TX_STATUS_RESPONSE_MAC_ACK_FAILURE 0x01
#define TX_STATUS_RESPONSE_CCA_FAILURE 0x02
#define TX_STATUS_RESPONSE_INVALID_DESTINATION_ENDPOINT 0x15
#define TX_STATUS_RESPONSE_NETWORK_ACK_FAILURE 0x21
#define TX_STATUS_RESPONSE_NOT_JOINED_TO_NETWORK 0x22
#define TX_STATUS_RESPONSE_SELF_ADDRESSED 0x23
#define TX_STATUS_RESPONSE_ADDRESS_NOT_FOUND 0x24
#define TX_STATUS_RESPONSE_ROUTE_NOT_FOUND 0x25
#define TX_STATUS_RESPONSE_BROADCAST_SOURCE_FAILED_TO_HEAR_A_NEIGHBOR_RELAY_THE_MESSAGE 0x26
#define TX_STATUS_RESPONSE_INVALID_BINDING_TABLE_INDEX 0x2B
#define TX_STATUS_RESPONSE_RESOURCE_ERROR 0x2C
#define TX_STATUS_RESPONSE_ATTEMPTED_BROADCAST_WITH_APS_TRANSMITION 0x2D
#define TX_STATUS_RESPONSE_ATTEMPTED_UNICAST_WITH_APS_TRANSMITION 0x2E
// #define TX_STATUS_RESPONSE_RESOURCE_ERROR 0x32
#define TX_STATUS_RESPONSE_DATA_PAYLOAD_TOO_LARGE 0x74
#define TX_STATUS_RESPONSE_INDIRECT_MESSAGE_UNREQUESTED 0x75

// TX_STATUS_RESPONSE_DISCOVERY_STATUS
#define TX_STATUS_RESPONSE_NO_DISCOVERY_OVERHEAD 0x00
#define TX_STATUS_RESPONSE_ADDRESS_DISCOVERY 0x01
#define TX_STATUS_RESPONSE_ROUTE_DISCOVERY 0x02
#define TX_STATUS_RESPONSE_ADDRESS_AND_ROUTE 0x03
#define TX_STATUS_RESPONSE_EXTENDED_TIMOUT_DISCOVERY 0x40

/* -------- Settings -------- */
#define FRAME_SIZE 110
#define MAX_PAYLOAD_SIZE 72
#define ERROR 0xFF

/* -------- Data Types -------- */
typedef unsigned char byte;

/* -------- Namespace -------- */
using namespace boost;

/* -------- XBeeFrame -------- */
class XBeeFrame
{
	public:
		XBeeFrame();
		virtual byte getData(byte field) = 0;
		virtual void setData(byte* data) = 0;
		virtual void setData(byte field, byte data) = 0;

	protected:
		byte startDelimiter;
		byte lengthMSB;
		byte lengthLSB;
		byte checksum;

		byte calculateChecksum();
		byte validateChecksum();
};

/* -------- XBeeRequest -------- */
class XBeeRequest : public XBeeFrame
{
	public:
		XBeeRequest();
};

/* -------- XBeeATCommandRequest -------- */
class XBeeATCommandRequest : public XBeeRequest
{
	public:
		XBeeATCommandRequest();
		XBeeATCommandRequest(byte frameId,byte atCommand0, byte atCommand1, byte parameterValue);
		XBeeATCommandRequest(byte frameId,byte atCommand0, byte atCommand1);
		byte getData(byte field);
		void setData(byte* data);
		void setData(byte field, byte data);

	protected:
		byte apiId;
		byte frameId;
		byte atCommand0;
		byte atCommand1;
		byte parameterValue;
};

/* -------- XBeeTXRequest -------- */
class XBeeTXRequest : public XBeeRequest
{
	public:
		XBeeTXRequest();
		XBeeTXRequest(byte frameId, unsigned long int addressMSB, unsigned long int addressLSB, byte networkAddress0, byte networkAddress1, byte broadcastRadius, byte options, byte rfDataSize);
		byte getData(byte field);
		void setData(byte* data);
		void setData(byte field, byte data);

	protected:
		byte apiId;
		byte frameId;
		byte destinationAddress0;
		byte destinationAddress1;
		byte destinationAddress2;
		byte destinationAddress3;
		byte destinationAddress4;
		byte destinationAddress5;
		byte destinationAddress6;
		byte destinationAddress7;
		byte networkAddress0;
		byte networkAddress1;
		byte broadcastRadius;
		byte options;
		byte rfData[MAX_PAYLOAD_SIZE];
};

/* ======== XBeeResponse ======== */
class XBeeResponse : public XBeeFrame
{
	public:
		XBeeResponse();
};

/* -------- XBeeATCommandResponse -------- */
class XBeeATCommandResponse : public XBeeResponse
{
	public:
		XBeeATCommandResponse();
		byte getData(byte field);
		void setData(byte* data);
		void setData(byte field, byte data);

	protected:
		byte apiId;
		byte frameId;
		byte atCommand0;
		byte atCommand1;
		byte commandStatus;
		byte commandData;
};

/* -------- XBeeRXResponse -------- */
class XBeeRXResponse : public XBeeResponse
{
	public:
		XBeeRXResponse();
		byte getData(byte field);
		void setData(byte* data);
		void setData(byte field, byte data);

	protected:
		byte apiId;
		byte sourceAddress0;
		byte sourceAddress1;
		byte sourceAddress2;
		byte sourceAddress3;
		byte sourceAddress4;
		byte sourceAddress5;
		byte sourceAddress6;
		byte sourceAddress7;
		byte networkAddress0;
		byte networkAddress1;
		byte receiveOptions;
		byte receiveData[MAX_PAYLOAD_SIZE];
};

/* -------- XBeeRXIOResponse -------- */
class XBeeRXIOResponse : public XBeeResponse
{
	public:
		XBeeRXIOResponse();
		byte getData(byte field);
		void setData(byte* data);
		void setData(byte field, byte data);

	protected:
		byte apiId;
		byte sourceAddress0;
		byte sourceAddress1;
		byte sourceAddress2;
		byte sourceAddress3;
		byte sourceAddress4;
		byte sourceAddress5;
		byte sourceAddress6;
		byte sourceAddress7;
		byte networkAddress0;
		byte networkAddress1;
		byte receiveOptions;
		byte numberOfSamples;
		byte digitalChannelMask0;
		byte digitalChannelMask1;
		byte analogChannelMask;
		byte digitalSamples0;
		byte digitalSamples1;
		byte analogSamples0;
		byte analogSamples1;
};

/* -------- XBeeStatusResponse -------- */
class XBeeTXStatusResponse : public XBeeResponse
{
	public:
		XBeeTXStatusResponse();
		byte getData(byte field);
		void setData(byte* data);
		void setData(byte field, byte data);

	protected:
		byte apiId;
		byte frameId;
		byte networkAddress0;
		byte networkAddress1;
		byte transmitRetryCount;
		byte deliveryStatus;
		byte discoveryStatus;
};

/* -------- XBee -------- */
class XBee
{
	public:
		XBee(const char* port, unsigned int baud);
		~XBee();
		void send(XBeeFrame* request); // send api frame
		byte receive(); // receive api frame
		byte* getResponse(); // get api frame

	private:
		SerialTimeout* serial;
		byte response[FRAME_SIZE]; // api frame

		void sendByte(byte b, bool escape);
};

#endif
