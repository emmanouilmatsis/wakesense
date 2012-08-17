#include <XBee.h>

XBee xbee;
XBeeTXRequest txRequest;

long timestamp;

void setup() {
  // Initialize the digital pin as an output.
  pinMode(13, OUTPUT); 

  // Init XBee
  xbee = XBee(Serial, 9600);
  
  // Assemble TX_REQUEST
  byte payloadSize = 0x06;
  txRequest = XBeeTXRequest(TX_REQUEST_DISABLE_ACK, 0x00000000, 0x00000000, 0xFF, 0xFE, 0x00, 0x00, payloadSize);
  int num = 7;
  txRequest.setData(TX_REQUEST_RF_DATA + 0x00, (0xFF & (num >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x01, (0xFF & (num >> 8))); 
  txRequest.setData(TX_REQUEST_RF_DATA + 0x02, (0xFF & (7 >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x03, (0xFF & (7 >> 8))); 
  txRequest.setData(TX_REQUEST_RF_DATA + 0x04, (0xFF & (7 >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x05, (0xFF & (7 >> 8))); 

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











