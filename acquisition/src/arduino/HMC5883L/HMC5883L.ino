#include <Wire.h>
#include <HMC5883L.h>


HMC5883L hmc; // variable hmc is an instance of the HMC5883L library
long timestamp; // magnetometer readings interval

void setup() {
  Serial.begin(9600);

  /* HMC5883L setup */
  // Continuous mode (default 10Hz)
  hmc.writeTo(HMC5883L_MODE, B00000000);

  // Data output rate (50Hz)
  hmc.writeTo(HMC5883L_CONFIG_A, B00011000);

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
    hmc.readMagn(xyz);
    
    // Output x,y,z values
    Serial.print("#MAG=");
    Serial.print(xyz[0]);
    Serial.print(",");
    Serial.print(xyz[1]);
    Serial.print(",");
    Serial.print(xyz[2]);
    Serial.println();
  }
}



