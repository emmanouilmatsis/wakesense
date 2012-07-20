
/* -------- Sensor initialisation -------- */

void I2C_Init()
{
  Wire.begin();
}

/* AXDL345 setup */
void Accel_Init()
{
  // Power register
  adxl.writeTo(ADXL345_POWER_CTL, B00000000);
  adxl.writeTo(ADXL345_POWER_CTL, B00001000);

  // Data format register
  adxl.writeTo(ADXL345_DATA_FORMAT, B00001011);

  // Rate register
  adxl.writeTo(ADXL345_BW_RATE, B00001001);
}

/* HMC5883L setup */
void Magn_Init()
{
  // Continuous mode (default 10Hz)
  hmc.writeTo(HMC5883L_MODE, B00000000);

  // Data output rate (50Hz)
  hmc.writeTo(HMC5883L_CONFIG_A, B00011000);
}

/* ITG3200 setup */
void Gyro_Init()
{
  // Power up reset defaults
  itg.writeTo(ITG3200_PWR_MGM, B10000000);

  // Select full-scale range of the gyro sensors
  // Set LP filter bandwidth to 42Hz
  itg.writeTo(ITG3200_DLPF_FS, B00011011);

  // Set sample rate to 50Hz
  itg.writeTo(ITG3200_SMPLRT_DIV, B00001010);

  // Set clock to PLL with z gyro reference
  itg.writeTo(ITG3200_PWR_MGM, B00000000);
}

// Read sensors
void read_sensors() {
  adxl.readAccel(accel);
  hmc.readMagn(magnetom);
  itg.readGyro(gyro);
}



