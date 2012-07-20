#include <Wire.h>
#include <ITG3200.h>


ITG3200 itg; // variable itg is an instance of the ITG3200 library
long timestamp; // magnetometer readings interval

void setup() {
  Serial.begin(9600);

  /* ITG3200 setup */
  // Power up reset defaults
  itg.writeTo(ITG3200_PWR_MGM, B10000000);
  
  // Select full-scale range of the gyro sensors
  // Set LP filter bandwidth to 42Hz
  itg.writeTo(ITG3200_DLPF_FS, B00011011);
  
  // Set sample rate to 50Hz
  itg.writeTo(ITG3200_SMPLRT_DIV, B00001010);

  // Set clock to PLL with z gyro reference
  itg.writeTo(ITG3200_PWR_MGM, B00000000);
  
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
    itg.readGyro(xyz);
    
    // Output x,y,z values
    Serial.print("#GYR=");
    Serial.print(xyz[0]);
    Serial.print(",");
    Serial.print(xyz[1]);
    Serial.print(",");
    Serial.print(xyz[2]);
    Serial.println();
  }
}



