
/* -------- Data output format -------- */

void output()
{
  short int yawDeg = TO_DEG(yaw) - TO_DEG(yawStart);
  short int pitchDeg = TO_DEG(pitch) - TO_DEG(pitchStart);
  short int rollDeg = TO_DEG(roll) - TO_DEG(rollStart);

  // < -180
  if (yawDeg < -180)
    yawDeg = 180 + (yawDeg % 180);
  if (pitchDeg < -180)
    pitchDeg = 180 + (pitchDeg % 180);
  if (rollDeg < -180)
    rollDeg = 180 + (rollDeg % 180);

  // > 180
  if (yawDeg > 180)
    yawDeg = -180 + (yawDeg % 180);
  if (pitchDeg > 180)
    pitchDeg = -180 + (pitchDeg % 180);
  if (rollDeg > 180)
    rollDeg = -180 + (rollDeg % 180);

  txRequest.setData(TX_REQUEST_RF_DATA + 0x00, (B11111111 & (yawDeg >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x01, (B11111111 & (yawDeg >> 8))); 
  txRequest.setData(TX_REQUEST_RF_DATA + 0x02, (B11111111 & (pitchDeg >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x03, (B11111111 & (pitchDeg >> 8))); 
  txRequest.setData(TX_REQUEST_RF_DATA + 0x04, (B11111111 & (rollDeg >> 0)));
  txRequest.setData(TX_REQUEST_RF_DATA + 0x05, (B11111111 & (rollDeg >> 8))); 

  xbee.send(&txRequest);
}




