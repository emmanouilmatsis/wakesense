#include <XBee.h>

XBee xbee;
XBeeTXRequest txRequest;
XBeeATCommandRequest atCommandRequest;
XBeeATCommandResponse atCommandResponse;

long timestamp;

void setup() {
  // Initialize the digital pin as an output.
  pinMode(13, OUTPUT); 

  // Init XBee
  xbee = XBee(Serial, 9600);

  // Assemble AT_COMMAND_REQUEST
  atCommandRequest = XBeeATCommandRequest(AT_COMMAND_REQUEST_ENABLE_ACK, 'A', 'P');

  // Send AT_COMMAND_REQUEST
  xbee.send(&atCommandRequest);

  // Receive AT_COMMAND_RESPONSE
  xbee.receive();
  atCommandResponse.setData(xbee.getResponse());
  
  // Assemble TX_REQUEST
  byte payloadSize = atCommandResponse.getData(FRAME_LENGTH_LSB) + 0x04;
  txRequest = XBeeTXRequest(TX_REQUEST_DISABLE_ACK, 0x00000000, 0x00000000, 0xFF, 0xFE, 0x00, 0x00, payloadSize);

  byte data;
  byte field = 0x00;
  while ((data = atCommandResponse.getData(field)) != ERROR) {
    txRequest.setData(TX_REQUEST_RF_DATA + field, data); 
    field++;
  }

  // Init variables
  timestamp = millis();
}

void loop() {
  if ((millis() - timestamp) >= 100) {
    timestamp = millis();

    // Send TX_REQUEST
    xbee.send(&txRequest);
  }
}











