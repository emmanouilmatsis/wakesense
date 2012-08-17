
/* -------- Data output format -------- */

void output()
{
  txRequest.setData(TX_REQUEST_RF_DATA + 0x00, (B11111111 & (((short int) TO_DEG(yaw)) >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x01, (B11111111 & (((short int) TO_DEG(yaw)) >> 8))); 
  txRequest.setData(TX_REQUEST_RF_DATA + 0x02, (B11111111 & (((short int) TO_DEG(pitch)) >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x03, (B11111111 & (((short int) TO_DEG(pitch)) >> 8))); 
  txRequest.setData(TX_REQUEST_RF_DATA + 0x04, (B11111111 & (((short int) TO_DEG(roll)) >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x05, (B11111111 & (((short int) TO_DEG(roll)) >> 8))); 
  
  xbee.send(&txRequest);
}


