#include <XBee.h>
#include <Wire.h>
#include <ADXL345.h>
#include <HMC5883L.h>
#include <ITG3200.h>

/* -------- Settings -------- */

// Set your serial port baud rate used to send out data here!
#define OUTPUT__BAUD_RATE 9600

// When set to true, gyro drift correction will not be applied
#define DEBUG__NO_DRIFT_CORRECTION false

// Airborn constants
#define SAMPLING_AIRBORN_DURATION 1000

// Sensor data output interval in milliseconds - minimum 20ms (50Hz)
#define SAMPLING_DATA_INTERVAL 20  // in milliseconds

/* -------- Constants -------- */

// Gain for accelerometer (ADXL345)
#define GRAVITY 256.0f // "1G reference" used for DCM filter and accelerometer calibration

// Gain for magnetometer (HMC5883L)
// --------

// Gain for gyroscope (ITG3200)
#define GYRO_GAIN 0.06957 // ITG3200 sensitivity factor 14.375 LSB/(degrees/sec) (1/14.375 = 0.06957)
#define GYRO_SCALED_RAD(x) (x * TO_RAD(GYRO_GAIN)) // calculate the scaled gyro readings in radians per second

// DCM constants
#define Kp_ROLLPITCH 0.02f
#define Ki_ROLLPITCH 0.00002f
#define Kp_YAW 1.2f
#define Ki_YAW 0.00002f

// PCC constants
#define PCC_SAMPLES 10
//#define PCC_THRESHOLD_MIN -0.3
//#define PCC_THRESHOLD_MAX 0.3
#define PCC_THRESHOLD_MIN -0.7
#define PCC_THRESHOLD_MAX 0.7

// Various
#define STATUS_LED_PIN 13  // pin number of status LED
#define TO_RAD(x) (x * 0.01745329252)  // *pi/180
#define TO_DEG(x) (x * 57.2957795131)  // *180/pi

/* -------- Global Variables -------- */

// Sensor variables
XBee xbee;
XBeeTXRequest txRequest;
ADXL345 adxl;
HMC5883L hmc;
ITG3200 itg;

// Sensor reading variables
float accel[3];
float magnetom[3];
float gyro[3]; 

// DCM variables
float MAG_Heading;
float Accel_Vector[3]= {0, 0, 0}; // store the acceleration in a vector
float Gyro_Vector[3]= {0, 0, 0}; // store the gyros turn rate in a vector
float Omega_Vector[3]= {0, 0, 0}; // corrected Gyro_Vector data
float Omega_P[3]= {0, 0, 0}; // Omega Proportional correction
float Omega_I[3]= {0, 0, 0}; // Omega Integrator
float Omega[3]= {0, 0, 0};
float errorRollPitch[3] = {0, 0, 0};
float errorYaw[3] = {0, 0, 0};
float DCM_Matrix[3][3] = {
  {1, 0, 0}, 
  {0, 1, 0},
  {0, 0, 1}
};
float Update_Matrix[3][3] = {
  {0, 1, 2},
  {3, 4, 5}, 
  {6, 7, 8}
};
float Temporary_Matrix[3][3] = {
  {0, 0, 0}, 
  {0, 0, 0}, 
  {0, 0, 0}
};

// Euler angles
float yaw;
float pitch;
float roll;

// PCC variables
float random_distribution[10];
float sample_distribution[10];

// Timing variables
long timestamp;
long timestamp_old;
float G_Dt; // integration time for DCM algorithm
boolean airborn;
long airborn_timestamp;

/* -------- Setup -------- */

void setup()
{
  xbee = XBee(Serial, OUTPUT__BAUD_RATE);
  txRequest = XBeeTXRequest(TX_REQUEST_DISABLE_ACK, 0x00000000, 0x00000000, 0xFF, 0xFE, 0x00, 0x00, 0x06);

  // Init status LED
  pinMode (STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  // Init sensors
  delay(50);  // give sensors enough time to start
  I2C_Init();
  Accel_Init();
  Magn_Init();
  Gyro_Init();

  // Init PCC
  delay(20); // give sensors enough time to collect data
  PCC_Init();
  
  // Init DCM algorithm
  delay(20); // give sensors enough time to collect data
  DCM_Init();
  
  // Init timing
  airborn = false;
}

/* -------- Loop -------- */

void loop()
{ 
  // Time to read the sensors again?
  if((millis() - timestamp) >= SAMPLING_DATA_INTERVAL)
  {
    timestamp_old = timestamp;
    timestamp = millis();
    if (timestamp > timestamp_old)
      G_Dt = (float) (timestamp - timestamp_old) / 1000.0f; // real time of loop run. We use this on the DCM algorithm (gyro integration time)
    else G_Dt = 0;

    // Update sensor readings
    read_sensors();

    // Run DCM algorithm
    Compass_Heading();
    Matrix_update();
    Normalize();
    Drift_correction(); 
    Euler_angles();

    // Update PPC
    PCC_Update();     

    // Run PCC algorithm (interval)
    ///if((PCC_THRESHOLD_MIN < PCC() && PCC() < PCC_THRESHOLD_MAX) || airborn) {
    if((PCC_THRESHOLD_MIN > PCC() || PCC() > PCC_THRESHOLD_MAX) || airborn) {

      // Output data for SAMPLING_AIRBORN_DURATION period
      airborn = true;  
      airborn_timestamp += SAMPLING_DATA_INTERVAL; // update aiborn timestamp
      output(); // output data
      if((airborn_timestamp) >= SAMPLING_AIRBORN_DURATION) {
        airborn = false;
        airborn_timestamp = 0;
      }
    }
  } 
}
