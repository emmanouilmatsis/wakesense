
/* -------- Data output format -------- */

void output()
{
  if (OUTPUT__MODE_ANGLES_BINARY)
  {
    float ypr[3];  
    ypr[0] = TO_DEG(yaw);
    ypr[1] = TO_DEG(pitch);
    ypr[2] = TO_DEG(roll);
    Serial.write((byte*) ypr, 12);  // No new-line
  }
  else
  {
      Serial.print("#YPR=");
      Serial.print(TO_DEG(yaw)); Serial.print(",");
      Serial.print(TO_DEG(pitch)); Serial.print(",");
      Serial.print(TO_DEG(roll)); Serial.println();
  }
}
