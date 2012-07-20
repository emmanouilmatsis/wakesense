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

byte XBeeATCommandRequest :: getData(byte field) {
	// Get field	
	if (field == FRAME_START_DELIMITER) return startDelimiter;
	else if (field == FRAME_LENGTH_MSB) return lengthMSB;
	else if (field == FRAME_LENGTH_LSB) return lengthLSB;
	else if (field == AT_COMMAND_REQUEST_API_ID) return apiId;
	else if (field == AT_COMMAND_REQUEST_FRAME_ID) return frameId;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_0) return atCommand0;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_1) return atCommand1;
	else if (field == AT_COMMAND_REQUEST_PARAMETER_VALUE) return parameterValue;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum;
	else return ERROR;
}

void XBeeATCommandRequest :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData(data[i], i);
}

void XBeeATCommandRequest :: setData(byte data, byte field) {
	// Set field	
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == AT_COMMAND_REQUEST_API_ID) apiId = data;
	else if (field == AT_COMMAND_REQUEST_FRAME_ID) frameId = data;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_0) atCommand0 = data;
	else if (field == AT_COMMAND_REQUEST_AT_COMMAND_1) atCommand1 = data;
	else if (field == AT_COMMAND_REQUEST_PARAMETER_VALUE) parameterValue = data;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data;
	else return;
}

/* -------- XBeeTXRequest -------- */

/* Public */

XBeeTXRequest :: XBeeTXRequest() : XBeeRequest() {
	// Invoke base constractor
}

XBeeTXRequest :: XBeeTXRequest(byte frameId, unsigned long int addressMSB, unsigned long int addressLSB, byte networkAddress0, byte networkAddress1, byte broadcastRadius, byte options, byte* rfData) : XBeeRequest() {
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
	for (unsigned int i = 0; i < sizeof(rfData); i++)
		this->rfData[i] = rfData[i];

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = ((14 + sizeof(rfData)) >> 8) & 0xFF;
	this->lengthLSB = ((14 + sizeof(rfData)) >> 0) & 0xFF;
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
	else if ((field >= TX_REQUEST_RF_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) return rfData[field]; 
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum;
	else return ERROR;
}

void XBeeTXRequest :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData(data[i], i);
}

void XBeeTXRequest :: setData(byte data, byte field) {
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
	else if ((field >= TX_REQUEST_RF_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) rfData[field] = data; 
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data;
	else return;
}

/* -------- XBeeResponse -------- */
XBeeResponse :: XBeeResponse() : XBeeFrame() {
	// Invoke base constractor 
}

/* -------- XBeeATCommandResponse -------- */

/* Public */

XBeeATCommandResponse :: XBeeATCommandResponse() : XBeeResponse() {
	// Invoke base constractor
}

XBeeATCommandResponse :: XBeeATCommandResponse(byte frameId, byte atCommand0, byte atCommand1, byte commandStatus, byte commandData) : XBeeResponse() {
	// Assemble AT_COMMAND_RESPONSE
	this->apiId = AT_COMMAND_RESPONSE;
	this->frameId = frameId;
	this->atCommand0 = atCommand0;
	this->atCommand1 = atCommand1;
	this->commandStatus = commandStatus;
	this->commandData = commandData;

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = 0x00;
	this->lengthLSB = 0x06;
	this->checksum = calculateChecksum();
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
	else if (field == AT_COMMAND_RESPONSE_COMMAND_DATA) return commandData;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum;
	else return ERROR;
}

void XBeeATCommandResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
  	setData(data[i], i);
}

void XBeeATCommandResponse :: setData(byte data, byte field) {
	// Set field
	if (field == FRAME_START_DELIMITER) startDelimiter = data;
	else if (field == FRAME_LENGTH_MSB) lengthMSB = data;
	else if (field == FRAME_LENGTH_LSB) lengthLSB = data;
	else if (field == AT_COMMAND_RESPONSE_API_ID) apiId = data;
	else if (field == AT_COMMAND_RESPONSE_FRAME_ID) frameId = data;
	else if (field == AT_COMMAND_RESPONSE_AT_COMMAND_0) atCommand0 = data;
	else if (field == AT_COMMAND_RESPONSE_AT_COMMAND_1) atCommand1 = data;
	else if (field == AT_COMMAND_RESPONSE_COMMAND_STATUS) commandStatus = data;
	else if (field == AT_COMMAND_RESPONSE_COMMAND_DATA) commandData = data;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data;
	else return;
}

/* -------- XBeeRXResponse -------- */

/* Public */

XBeeRXResponse :: XBeeRXResponse() : XBeeResponse() {
	// Invoke base constractor
}

XBeeRXResponse :: XBeeRXResponse(unsigned long int addressMSB, unsigned long int addressLSB, byte networkAddress0, byte networkAddress1, byte receiveOptions, byte* receiveData) : XBeeResponse() {
	// Assemble RX_RESPONSE
	this->apiId = RX_RESPONSE;
	this->sourceAddress0 = (addressMSB >> 24) & 0xFF;
	this->sourceAddress1 = (addressMSB >> 16) & 0xFF;
	this->sourceAddress2 = (addressMSB >> 8) & 0xFF;  
	this->sourceAddress3 = (addressMSB >> 0) & 0xFF;  
	this->sourceAddress4 = (addressLSB >> 24) & 0xFF; 
	this->sourceAddress5 = (addressLSB >> 16) & 0xFF;
	this->sourceAddress6 = (addressLSB >> 8) & 0xFF;  
	this->sourceAddress7 = (addressLSB >> 0) & 0xFF;  
	this->networkAddress0 = networkAddress0;
	this->networkAddress1 = networkAddress1;
	this->receiveOptions = receiveOptions;
	for (unsigned int i = 0; i < sizeof(receiveData); i++)
		this->receiveData[i] = receiveData[i];

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = ((12 + sizeof(receiveData)) >> 8) & 0xFF; 
	this->lengthLSB = ((12 + sizeof(receiveData)) >> 0) & 0xFF; 
	this->checksum = calculateChecksum();
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
	else if ((field >= RX_RESPONSE_RECEIVE_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) return receiveData[field]; 
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum;
	else return ERROR;
}

void XBeeRXResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData(data[i], i);
}

void XBeeRXResponse :: setData(byte data, byte field) {
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
	else if ((field >= RX_RESPONSE_RECEIVE_DATA) && (field < ((lengthMSB << 8) + lengthLSB) + 3)) receiveData[field] = data; 
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data;
	else return;
}

/* -------- XBeeRXIOResponse -------- */

/* Public */

XBeeRXIOResponse :: XBeeRXIOResponse() : XBeeResponse() {
	// Invoke base constractor
}

XBeeRXIOResponse :: XBeeRXIOResponse(unsigned long int addressMSB, unsigned long int addressLSB, byte networkAddress0, byte networkAddress1, byte receiveOptions, byte numberOfSamples, byte digitalChannelMask0, byte digitalChannelMask1, byte analogChannelMask, byte digitalSamples0, byte digitalSamples1, byte analogSamples0, byte analogSamples1) : XBeeResponse() {
	// Assemble RX_IO_RESPONSE
	this->apiId = RX_IO_RESPONSE;
	this->sourceAddress0 = (addressMSB >> 24) & 0xFF;
	this->sourceAddress1 = (addressMSB >> 16) & 0xFF;
	this->sourceAddress2 = (addressMSB >> 8) & 0xFF;  
	this->sourceAddress3 = (addressMSB >> 0) & 0xFF;  
	this->sourceAddress4 = (addressLSB >> 24) & 0xFF; 
	this->sourceAddress5 = (addressLSB >> 16) & 0xFF;
	this->sourceAddress6 = (addressLSB >> 8) & 0xFF;  
	this->sourceAddress7 = (addressLSB >> 0) & 0xFF;  
	this->networkAddress0 = networkAddress0;
	this->networkAddress1 = networkAddress1;
	this->receiveOptions = receiveOptions;
	this->numberOfSamples = numberOfSamples;
	this->digitalChannelMask0 = digitalChannelMask0;
	this->digitalChannelMask1 = digitalChannelMask1;
	this->analogChannelMask = analogChannelMask;
	this->digitalSamples0 = digitalSamples0;
	this->digitalSamples1 = digitalSamples1;
	this->analogSamples0 = analogSamples0;
	this->analogSamples1 = analogSamples1;

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = 0x00;
	this->lengthLSB = 0x20; 
	this->checksum = calculateChecksum();
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
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_0) return digitalSamples0;
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_1) return digitalSamples1;
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_0) return analogSamples0;
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_1) return analogSamples1;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum;
	else return ERROR;
}

void XBeeRXIOResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData(data[i], i);
}

void XBeeRXIOResponse :: setData(byte data, byte field) {
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
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_0) digitalSamples0 = data;
	else if (field == RX_IO_RESPONSE_DIGITAL_SAMPLES_1) digitalSamples1 = data;
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_0) analogSamples0 = data;
	else if (field == RX_IO_RESPONSE_ANALOG_SAMPLES_1) analogSamples1 = data;
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data;
	else return;
}

/* -------- XBeeTXStatusResponse -------- */

/* Public */

XBeeTXStatusResponse :: XBeeTXStatusResponse() : XBeeResponse() {
	// Invoke base constractor
}

XBeeTXStatusResponse :: XBeeTXStatusResponse(byte frameId, byte networkAddress0, byte networkAddress1, byte transmitRetryCount, byte deliveryStatus, byte discoveryStatus) : XBeeResponse() {
	// Assemble TX_STATUS_RESPONSE
	this->apiId = TX_STATUS_RESPONSE;
	this->frameId = frameId;
	this->networkAddress0 = networkAddress0;
	this->networkAddress1 = networkAddress1;
	this->transmitRetryCount = transmitRetryCount;
	this->deliveryStatus = deliveryStatus;
	this->discoveryStatus = discoveryStatus;

	// Assemble FRAME
	this->startDelimiter = START_DELIMITER;
	this->lengthMSB = 0x00;
	this->lengthLSB = 0x07;
	this->checksum = calculateChecksum();
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
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) return checksum;
	else return ERROR;
}

void XBeeTXStatusResponse :: setData(byte* data) {
	for (int i = 0; i < ((data[1] << 8) + data[2]) + 4; i++)
		setData(data[i], i);
}

void XBeeTXStatusResponse :: setData(byte data, byte field) {
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
	else if ((field == FRAME_CHECKSUM) || (field == (((lengthMSB << 8) + lengthLSB) + 3))) checksum = data;
	else return;
}

/* -------- XBee -------- */

/* Public */

XBee :: XBee(HardwareSerial& serial, long baud) {
	// Setup serial
	// this->serial = &serial;
	// this->serial->begin(baud);
}

void XBee :: send(XBeeFrame* request) {
	// Send frame bytes
	sendByte(request->getData(FRAME_START_DELIMITER), false); // disable escape (START_DELIMITER)
	sendByte(request->getData(FRAME_LENGTH_MSB), true); // enable escape (LENGTH_MSB)
	sendByte(request->getData(FRAME_LENGTH_LSB), true); // enable escape (LENGTH_LSB)
	for (int i = 0; i < ((request->getData(FRAME_LENGTH_MSB) << 8) + request->getData(FRAME_LENGTH_LSB)); i++)
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

			// Read escaped byte
			if (field > 0 && b == ESCAPE) {
				while (!serial->available()) {} // poll until escaped byte is available
				b = serial->read();
				b = 0x20 ^ b;
			}

			// Set byte
			if (field == 0) { // waiting frame
				if (b == START_DELIMITER) { // new frame
					response[field] = b;
					field++;

				} else if (b != START_DELIMITER) { // no frame
					continue;
				} 

			} else if (field > 0) { // reading frame
				if (b == START_DELIMITER) { 
					return ERROR; // incomplete frame

				} else if (b != START_DELIMITER) { // current frame
					if (field > FRAME_SIZE)
						return ERROR; // oversized frame

					// Update buffer
					response[field] = b; // set byte
					checksum += b; // set checksum
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
}

byte* XBee :: getResponse() {
	// Return frame buffer
	return response; // frame->setData(getFrame());
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
