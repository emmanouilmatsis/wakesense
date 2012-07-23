#include "XBee.h"

/* -------- XBeeFrame -------- */

/* Public */

XBeeFrame :: XBeeFrame() {
	// Do nothing
}

/* Private */

byte XBeeFrame :: calculateChecksum() {
	checksum = 0; // reset checksum

	for (int i = 3; i < ((lengthMSB << 8) + lengthLSB) + 3; i++) { // START_DELIMITER + LENGTH_MSB + LENGTH_LSB = +3
		checksum += getData(i); 
	}
	checksum = 0xFF - checksum;
	return checksum;
}

byte XBeeFrame :: validateChecksum() {
	checksum = 0; // reset checksum

	for (int i = 3; i < ((lengthMSB << 8) + lengthLSB) + 4; i++) { // START_DELIMITER + LENGTH_MSB + LENGTH_LSB + CHECKSUM = +4
		if (getData(i) == ESCAPE) { 
			checksum += 0x20 ^ getData(i + 1);
			i++;
			continue;
		}
		checksum += getData(i); 
	}
	return checksum;
}

/* -------- XBeeRequest -------- */
XBeeRequest :: XBeeRequest() : XBeeFrame() {
	// Invoke base constractor
}

/* -------- XBeeATCommandRequest -------- */

/* Public */

XBeeATCommandRequest :: XBeeATCommandRequest() : XBeeRequest() {
	// Invoke base constractor
}

XBeeATCommandRequest :: XBeeATCommandRequest(byte frameId, byte atCommand0, byte atCommand1, byte parameterValue) : XBeeRequest() {
	// Assemble AT_COMMAND_REQUEST
	this->apiId = AT_COMMAND_REQUEST;
	this->frameId = frameId;
	this->atCommand0 = atCommand0;
	this->atCommand1 = atCommand1;
	this->parameterValue = parameterValue;

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = 0x00;
	this->lengthLSB = 0x05;
	this->checksum = calculateChecksum();
}

XBeeATCommandRequest :: XBeeATCommandRequest(byte frameId, byte atCommand0, byte atCommand1) : XBeeRequest() {
	// Assemble AT_COMMAND_REQUEST
	this->apiId = AT_COMMAND_REQUEST;
	this->frameId = frameId;
	this->atCommand0 = atCommand0;
	this->atCommand1 = atCommand1;

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = 0x00;
	this->lengthLSB = 0x04;
	this->checksum = calculateChecksum();
}

byte XBeeATCommandRequest :: getData(byte field) {
	// Get field	
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == AT_COMMAND_REQUEST_API_ID) return apiId;
	else if (field == AT_COMMAND_REQUEST_FRAME_ID) return frameId;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_0) return atCommand0;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_1) return atCommand1;
	else if (field == AT_COMMAND_REQUEST_PARAMETER_VALUE && lengthLSB == 0x05) return parameterValue; // TODO optional field
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum; // TODO error if lenghtMSB and lengthLSB are undefined
	else return ERROR;
}

void XBeeATCommandRequest :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData((i & 0xFF), data[i]);
}

void XBeeATCommandRequest :: setData(byte field, byte data) {
	// Set field	
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == AT_COMMAND_REQUEST_API_ID) apiId = data;
	else if (field == AT_COMMAND_REQUEST_FRAME_ID) frameId = data;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_0) atCommand0 = data;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_1) atCommand1 = data;
	else if (field == AT_COMMAND_REQUEST_PARAMETER_VALUE && lengthLSB == 0x05) parameterValue = data; // TODO optional field
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data; // TODO error if lenghtMSB and lengthLSB are undefined // TODO always overridden
	else return;

	// Calculate checksum
	this->checksum = calculateChecksum();
}

/* -------- XBeeTXRequest -------- */

/* Public */

XBeeTXRequest :: XBeeTXRequest() : XBeeRequest() {
	// Invoke base constractor
}

XBeeTXRequest :: XBeeTXRequest(byte frameId, unsigned long int addressMSB, unsigned long int addressLSB, byte networkAddress0, byte networkAddress1, byte broadcastRadius, byte options, byte rfDataSize) : XBeeRequest() {
	// Assemble TX_REQUEST
	this->apiId = TX_REQUEST;
	this->frameId = frameId;
	this->destinationAddress0 = (addressMSB >> 24) & 0xFF;
	this->destinationAddress1 = (addressMSB >> 16) & 0xFF;
	this->destinationAddress2 = (addressMSB >> 8) & 0xFF;
	this->destinationAddress3 = (addressMSB >> 0) & 0xFF;
	this->destinationAddress4 = (addressLSB >> 24) & 0xFF;
	this->destinationAddress5 = (addressLSB >> 16) & 0xFF;
	this->destinationAddress6 = (addressLSB >> 8) & 0xFF;
	this->destinationAddress7 = (addressLSB >> 0) & 0xFF;
	this->networkAddress0 = networkAddress0;
	this->networkAddress1 = networkAddress1;
	this->broadcastRadius = broadcastRadius;
	this->options = options;
	for (unsigned int i = 0; i < rfDataSize; i++)
		this->rfData[i] = 0x00; // initialize with 0x00

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = ((rfDataSize + 14) >> 8) & 0xFF;
	this->lengthLSB = ((rfDataSize + 14) >> 0) & 0xFF;
	this->checksum = calculateChecksum();
}

byte XBeeTXRequest :: getData(byte field) {
	// Get field
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == TX_REQUEST_API_ID) return apiId;
	else if (field == TX_REQUEST_FRAME_ID) return frameId;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_0) return destinationAddress0;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_1) return destinationAddress1;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_2) return destinationAddress2;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_3) return destinationAddress3;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_4) return destinationAddress4;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_5) return destinationAddress5;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_6) return destinationAddress6;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_7) return destinationAddress7;
	else if (field == TX_REQUEST_NETWORK_ADDRESS_0) return networkAddress0;
	else if (field == TX_REQUEST_NETWORK_ADDRESS_1) return networkAddress1;
	else if (field == TX_REQUEST_BROADCAST_RADIUS) return broadcastRadius;
	else if (field == TX_REQUEST_OPTIONS) return options;
	else if ((field >= TX_REQUEST_RF_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) return rfData[field - TX_REQUEST_RF_DATA]; // TODO error if lenghtMSB and lengthLSB are undefined
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum; // TODO error if lenghtMSB and lengthLSB are undefined
	else return ERROR;
}

void XBeeTXRequest :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData((i & 0xFF), data[i]);
}

void XBeeTXRequest :: setData(byte field, byte data) {
	// Set field
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == TX_REQUEST_API_ID) apiId = data;
	else if (field == TX_REQUEST_FRAME_ID) frameId = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_0) destinationAddress0 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_1) destinationAddress1 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_2) destinationAddress2 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_3) destinationAddress3 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_4) destinationAddress4 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_5) destinationAddress5 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_6) destinationAddress6 = data;
	else if (field == TX_REQUEST_DESTINATION_ADDRESS_7) destinationAddress7 = data;
	else if (field == TX_REQUEST_NETWORK_ADDRESS_0) networkAddress0 = data;
	else if (field == TX_REQUEST_NETWORK_ADDRESS_1) networkAddress1 = data;
	else if (field == TX_REQUEST_BROADCAST_RADIUS) broadcastRadius = data;
	else if (field == TX_REQUEST_OPTIONS) options = data;
	else if ((field >= TX_REQUEST_RF_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) rfData[field - TX_REQUEST_RF_DATA] = data; // TODO error if lenghtMSB and lengthLSB are undefined
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data; // TODO error if lenghtMSB and lengthLSB are undefined // TODO always overridden
	else return;

	// Calculate checksum
	this->checksum = calculateChecksum();
}

/* ======== XBeeResponse ======== */

/* Public */ 

XBeeResponse :: XBeeResponse() : XBeeFrame() {
	// Invoke base constractor 
}

/* -------- XBeeATCommandResponse -------- */

/* Public */

XBeeATCommandResponse :: XBeeATCommandResponse() : XBeeResponse() {
	// Invoke base constractor
}

byte XBeeATCommandResponse :: getData(byte field) {
	// Get field
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == AT_COMMAND_RESPONSE_API_ID) return apiId;
	else if (field == AT_COMMAND_RESPONSE_FRAME_ID) return frameId;
	else if (field == AT_COMMAND_RESPONSE_AT_COMMAND_0) return atCommand0;
	else if (field == AT_COMMAND_RESPONSE_AT_COMMAND_1) return atCommand1;
	else if (field == AT_COMMAND_RESPONSE_COMMAND_STATUS) return commandStatus;
	else if (field == AT_COMMAND_RESPONSE_COMMAND_DATA && lengthLSB == 0x06) return commandData; // TODO optional field
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum; // TODO error if lenghtMSB and lengthLSB are undefined
	else return ERROR;
}


void XBeeATCommandResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData((i & 0xFF), data[i]);
}

void XBeeATCommandResponse :: setData(byte field, byte data) {
	// Set field
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == AT_COMMAND_RESPONSE_API_ID) apiId = data;
	else if (field == AT_COMMAND_RESPONSE_FRAME_ID) frameId = data;
	else if (field == AT_COMMAND_RESPONSE_AT_COMMAND_0) atCommand0 = data;
	else if (field == AT_COMMAND_RESPONSE_AT_COMMAND_1) atCommand1 = data;
	else if (field == AT_COMMAND_RESPONSE_COMMAND_STATUS) commandStatus = data;
	else if (field == AT_COMMAND_RESPONSE_COMMAND_DATA && lengthLSB == 0x06) commandData = data; // TODO optional field
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data; // TODO error if lenghtMSB and lengthLSB are undefined // TODO always overridden
	else return;

	// Calculate checksum;
	this->checksum = calculateChecksum();
}

/* -------- XBeeRXResponse -------- */

/* Public */

XBeeRXResponse :: XBeeRXResponse() : XBeeResponse() {
	// Invoke base constractor
}

byte XBeeRXResponse :: getData(byte field) {
	// Get field
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == RX_RESPONSE_API_ID) return apiId;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_0) return sourceAddress0;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_1) return sourceAddress1;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_2) return sourceAddress2;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_3) return sourceAddress3;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_4) return sourceAddress4;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_5) return sourceAddress5;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_6) return sourceAddress6;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_7) return sourceAddress7;
	else if (field == RX_RESPONSE_NETWORK_ADDRESS_0) return networkAddress0;
	else if (field == RX_RESPONSE_NETWORK_ADDRESS_1) return networkAddress1;
	else if (field == RX_RESPONSE_RECEIVE_OPTIONS) return receiveOptions;
	else if ((field >= RX_RESPONSE_RECEIVE_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) return receiveData[field - RX_RESPONSE_RECEIVE_DATA]; // TODO error if lenghtMSB and lengthLSB are undefined
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum; // TODO error if lenghtMSB and lengthLSB are undefined
	else return ERROR;
}

void XBeeRXResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData((i & 0xFF), data[i]);
}

void XBeeRXResponse :: setData(byte field, byte data) {
	// Set field
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == RX_RESPONSE_API_ID) apiId = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_0) sourceAddress0 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_1) sourceAddress1 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_2) sourceAddress2 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_3) sourceAddress3 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_4) sourceAddress4 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_5) sourceAddress5 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_6) sourceAddress6 = data;
	else if (field == RX_RESPONSE_SOURCE_ADDRESS_7) sourceAddress7 = data;
	else if (field == RX_RESPONSE_NETWORK_ADDRESS_0) networkAddress0 = data;
	else if (field == RX_RESPONSE_NETWORK_ADDRESS_1) networkAddress1 = data;
	else if (field == RX_RESPONSE_RECEIVE_OPTIONS) receiveOptions = data;
	else if ((field >= RX_RESPONSE_RECEIVE_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) receiveData[field - RX_RESPONSE_RECEIVE_DATA] = data; // TODO error if lenghtMSB and lengthLSB are undefined
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data; // TODO error if lenghtMSB and lengthLSB are undefined // TODO always overridden
	else return;

	// Calculate checksum
	this->checksum = calculateChecksum();
}

/* -------- XBeeRXIOResponse -------- */

/* Public */

XBeeRXIOResponse :: XBeeRXIOResponse() : XBeeResponse() {
	// Invoke base constractor
}

byte XBeeRXIOResponse :: getData(byte field) {
	// Get field
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == RX_IO_RESPONSE_API_ID) return apiId;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_0) return sourceAddress0;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_1) return sourceAddress1;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_2) return sourceAddress2;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_3) return sourceAddress3;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_4) return sourceAddress4;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_5) return sourceAddress5;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_6) return sourceAddress6;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_7) return sourceAddress7;
	else if (field == RX_IO_RESPONSE_NETWORK_ADDRESS_0) return networkAddress0;
	else if (field == RX_IO_RESPONSE_NETWORK_ADDRESS_1) return networkAddress1;
	else if (field == RX_IO_RESPONSE_RECEIVE_OPTIONS) return receiveOptions;
	else if (field == RX_IO_RESPONSE_NUMBER_OF_SAMPLES) return numberOfSamples;
	else if (field == RX_IO_RESPONSE_DIGITAL_CHANNEL_MASK_0) return digitalChannelMask0;
	else if (field == RX_IO_RESPONSE_DIGITAL_CHANNEL_MASK_1) return digitalChannelMask1;
	else if (field == RX_IO_RESPONSE_ANALOG_CHANNEL_MASK) return analogChannelMask;
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_0) return digitalSamples0; // TODO optional field 
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_1) return digitalSamples1; // TODO optional field
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_0) return analogSamples0;
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_1) return analogSamples1;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum; // TODO error if lenghtMSB and lengthLSB are undefined
	else return ERROR;
}

void XBeeRXIOResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData((i & 0xFF), data[i]);
}

void XBeeRXIOResponse :: setData(byte field, byte data) {
	// Set field
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == RX_IO_RESPONSE_API_ID) apiId = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_0) sourceAddress0 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_1) sourceAddress1 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_2) sourceAddress2 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_3) sourceAddress3 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_4) sourceAddress4 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_5) sourceAddress5 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_6) sourceAddress6 = data;
	else if (field == RX_IO_RESPONSE_SOURCE_ADDRESS_7) sourceAddress7 = data;
	else if (field == RX_IO_RESPONSE_NETWORK_ADDRESS_0) networkAddress0 = data;
	else if (field == RX_IO_RESPONSE_NETWORK_ADDRESS_1) networkAddress1 = data;
	else if (field == RX_IO_RESPONSE_RECEIVE_OPTIONS) receiveOptions = data;
	else if (field == RX_IO_RESPONSE_NUMBER_OF_SAMPLES) numberOfSamples = data;
	else if (field == RX_IO_RESPONSE_DIGITAL_CHANNEL_MASK_0) digitalChannelMask0 = data;
	else if (field == RX_IO_RESPONSE_DIGITAL_CHANNEL_MASK_1) digitalChannelMask1 = data;
	else if (field == RX_IO_RESPONSE_ANALOG_CHANNEL_MASK) analogChannelMask = data;
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_0) digitalSamples0 = data; // TODO optional field
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_1) digitalSamples1 = data; // TODO optional field
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_0) analogSamples0 = data;
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_1) analogSamples1 = data;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data; // TODO error if lenghtMSB and lengthLSB are undefined // TODO always overridden
	else return;

	// Calculate checksum
	this->checksum = calculateChecksum();
}

/* -------- XBeeTXStatusResponse -------- */

/* Public */

XBeeTXStatusResponse :: XBeeTXStatusResponse() : XBeeResponse() {
	// Invoke base constractor
}

byte XBeeTXStatusResponse :: getData(byte field) {
	// Get field
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == TX_STATUS_RESPONSE_API_ID) return apiId;
	else if (field == TX_STATUS_RESPONSE_FRAME_ID) return frameId;
	else if (field == TX_STATUS_RESPONSE_NETWORK_ADDRESS_0) return networkAddress0;
	else if (field == TX_STATUS_RESPONSE_NETWORK_ADDRESS_1) return networkAddress1;
	else if (field == TX_STATUS_RESPONSE_TRANSMIT_RETRY_COUNT) return transmitRetryCount;
	else if (field == TX_STATUS_RESPONSE_DELIVERY_STATUS) return deliveryStatus;
	else if (field == TX_STATUS_RESPONSE_DISCOVERY_STATUS) return discoveryStatus;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum; // TODO error if lenghtMSB and lengthLSB are undefined
	else return ERROR;
}

void XBeeTXStatusResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData((i & 0xFF), data[i]);
}

void XBeeTXStatusResponse :: setData(byte field, byte data) {
	// Set field
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == TX_STATUS_RESPONSE_API_ID) apiId = data;
	else if (field == TX_STATUS_RESPONSE_FRAME_ID) frameId = data;
	else if (field == TX_STATUS_RESPONSE_NETWORK_ADDRESS_0) networkAddress0 = data;
	else if (field == TX_STATUS_RESPONSE_NETWORK_ADDRESS_1) networkAddress1 = data;
	else if (field == TX_STATUS_RESPONSE_TRANSMIT_RETRY_COUNT) transmitRetryCount = data;
	else if (field == TX_STATUS_RESPONSE_DELIVERY_STATUS) deliveryStatus = data;
	else if (field == TX_STATUS_RESPONSE_DISCOVERY_STATUS) discoveryStatus = data;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data; // TODO error if lengthMSB and lengthLSB are undefined // TODO always overridden
	else return;

	// Calculate checksum
	this->checksum = calculateChecksum();
}

/* -------- XBee -------- */

/* Public */
XBee :: XBee() {
	// Do nothing
}

XBee :: XBee(HardwareSerial& serial, long baud) {
	// Setup serial
	this->serial = &serial;
	this->serial->begin(baud);
}

void XBee :: send(XBeeFrame* request) {
	// Send frame bytes
	sendByte(request->getData(FRAME_START_DELIMITER), false); // disable escape (START_DELIMITER)
	sendByte(request->getData(FRAME_LENGTH_MSB), true); // enable escape (LENGTH_MSB)
	sendByte(request->getData(FRAME_LENGTH_LSB), true); // enable escape (LENGTH_LSB)
	for (int i = 0; i < ((request->getData(FRAME_LENGTH_MSB) << 8) | request->getData(FRAME_LENGTH_LSB)); i++)
		sendByte(request->getData(i + 3), true); //enable escape (FRAME_DATA)
	sendByte(request->getData(FRAME_CHECKSUM), true); // enable escape (CHECKSUM)

	serial->flush(); // wait for the transmission of outgoing serial data to complete
}

byte XBee :: receive() {
	unsigned int field = 0; // reset frame field pointer
	byte checksum = 0; // reset checksum

	// Poll until frame is received
	while (true) {
		// Read frame

		while (serial->available()) {
			// Read byte
			byte b = serial->read();

			// Set byte
			if (field == 0) { // waiting frame
				if (b == START_DELIMITER) { // new frame
					response[field] = b;
					field++;
				} else { // no frame
					continue;
				} 
			} else if (field > 0) { // reading frame
				// Incomplete frame
				if (b == START_DELIMITER) 
					return ERROR;

				// Read escaped byte
				if (b == ESCAPE) {
					while (!serial->available()) {} // poll until escaped byte is available
					b = serial->read();
					b = 0x20 ^ b;
				}

				// Ovesized frame
				if (field > FRAME_SIZE)
					return ERROR; // oversized frame

				// Update buffer
				response[field] = b; // set byte
				if (field >= 3) checksum += b; // set checksum
				field++;

				// Complete frame
				/*
					 field = [0..n]
					 length = [1..n]
					 START_DELIMITER + LENGTH_MSB + LENGTH_LSB + CHECKSUM = 4
					 --------
					 max(field) + 1 = length + 4
				 */
				if (static_cast<int>(field) == (((response[1] << 8) + response[2]) + 4)) {
					// Validate checksum
					if ((checksum & 0xFF) == 0xFF) {
						return response[3]; // return API_ID
					} else {
						return ERROR; // corrupted frame
					}
				}
			}
		}
	}   
}

byte* XBee :: getResponse() {
	// Return frame buffer
	return response; // frame->setData(getResponse());
}

/* Private */

void XBee :: sendByte(byte b, bool escape) {
	// Send byte to serial
	if (escape && (b == START_DELIMITER || b == ESCAPE || b == ON || b == OFF)) { 
		serial->write(ESCAPE);
		serial->write(b ^ 0x20);
	} else { 
		serial->write(b);
	}
}
