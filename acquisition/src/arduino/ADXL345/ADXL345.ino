#include <Wire.h>
#include <ADXL345.h>


ADXL345 adxl; // variable adxl is an instance of the ADXL345 library
long timestamp; // acceleration readings interval

void setup() {
  Serial.begin(9600);

  /* AXDL345 setup */
  // Power register
  adxl.writeTo(ADXL345_POWER_CTL, B00000000);
  adxl.writeTo(ADXL345_POWER_CTL, B00001000);

  // Data format register
  adxl.writeTo(ADXL345_DATA_FORMAT, B00001011);

  // Rate register
  adxl.writeTo(ADXL345_BW_RATE, B00001001);

  /* Various */
  // Timestamp
  timestamp = millis();
}

void loop() {

  // Time to read the sensors again?
  if((millis() - timestamp) >= 100)
  {
    timestamp = millis();

    int xyz[3];
    adxl.readAccel(xyz);
    
    double xyzG[3];
    adxl.readGxyz(xyzG);
    
    double g;
    adxl.readG(&g);
    
    // Output x,y,z values
    Serial.print("#ACC=");
    Serial.print(xyz[0]);
    Serial.print(",");
    Serial.print(xyz[1]);
    Serial.print(",");
    Serial.print(xyz[2]);
    Serial.print("  -  ");
    Serial.print("#GXYZ=");
    Serial.print(xyzG[0]);
    Serial.print(",");
    Serial.print(xyzG[1]);
    Serial.print(",");
    Serial.print(xyzG[2]);
    Serial.print("  -  ");
    Serial.print("#G=");
    Serial.print(g);
    Serial.println();
  }
}



