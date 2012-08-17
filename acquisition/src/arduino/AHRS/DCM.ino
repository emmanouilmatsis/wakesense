
/* -------- DCM algorithm -------- */

// Initialise DCM with unfiltered orientation
void DCM_Init() {
  float temp1[3];
  float temp2[3];
  float xAxis[] = {1.0f, 0.0f, 0.0f};

  // Read every sensor and record a time stamp
  read_sensors();
  timestamp = millis();

  // GET PITCH
  // Using y-z-plane-component/x-component of gravity vector
  pitch = -atan2(accel[0], sqrt(accel[1] * accel[1] + accel[2] * accel[2]));

  // GET ROLL
  // Compensate pitch of gravity vector 
  Vector_Cross_Product(temp1, accel, xAxis);
  Vector_Cross_Product(temp2, xAxis, temp1);
  // Normally using x-z-plane-component/y-component of compensated gravity vector
  // roll = atan2(temp2[1], sqrt(temp2[0] * temp2[0] + temp2[2] * temp2[2]));
  // Since we compensated for pitch, x-z-plane-component equals z-component:
  roll = atan2(temp2[1], temp2[2]);

  // GET YAW
  Compass_Heading();
  yaw = MAG_Heading;

  // Init rotation matrix
  init_rotation_matrix(DCM_Matrix, yaw, pitch, roll);
}

// Matrix update
void Matrix_update(void)
{
  Gyro_Vector[0] = GYRO_SCALED_RAD(gyro[0]); // gyro x roll
  Gyro_Vector[1] = GYRO_SCALED_RAD(gyro[1]); // gyro y pitch
  Gyro_Vector[2] = GYRO_SCALED_RAD(gyro[2]); // gyro z yaw
  
  Accel_Vector[0] = accel[0];
  Accel_Vector[1] = accel[1];
  Accel_Vector[2] = accel[2];
    
  Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);  // adding proportional term
  Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); // adding integrator term
  
#if DEBUG__NO_DRIFT_CORRECTION == true // Do not use drift correction
  Update_Matrix[0][0] = 0;
  Update_Matrix[0][1] = -G_Dt * Gyro_Vector[2];// -z
  Update_Matrix[0][2] = G_Dt * Gyro_Vector[1];// y
  Update_Matrix[1][0] = G_Dt * Gyro_Vector[2];// z
  Update_Matrix[1][1] = 0;
  Update_Matrix[1][2] = -G_Dt * Gyro_Vector[0];
  Update_Matrix[2][0] = -G_Dt * Gyro_Vector[1];
  Update_Matrix[2][1] = G_Dt * Gyro_Vector[0];
  Update_Matrix[2][2] = 0;
#else // Use drift correction
  Update_Matrix[0][0] = 0;
  Update_Matrix[0][1] = -G_Dt * Omega_Vector[2];// -z
  Update_Matrix[0][2] = G_Dt * Omega_Vector[1];// y
  Update_Matrix[1][0] = G_Dt * Omega_Vector[2];// z
  Update_Matrix[1][1] = 0;
  Update_Matrix[1][2] = -G_Dt * Omega_Vector[0];// -x
  Update_Matrix[2][0] = -G_Dt * Omega_Vector[1];// -y
  Update_Matrix[2][1] = G_Dt * Omega_Vector[0];// x
  Update_Matrix[2][2] = 0;
#endif

  Matrix_Multiply(DCM_Matrix, Update_Matrix, Temporary_Matrix); // a*b=c

  for(int x=0; x<3; x++) // Matrix Addition (update)
  {
    for(int y=0; y<3; y++)
    {
      DCM_Matrix[x][y] += Temporary_Matrix[x][y];
    } 
  }
}

// Normalize
void Normalize(void)
{
  float error = 0;
  float temporary[3][3];
  float renorm = 0;
  
  error = -Vector_Dot_Product(&DCM_Matrix[0][0], &DCM_Matrix[1][0]) * 0.5; // eq.19

  Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); // eq.19
  Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); // eq.19
  
  Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);// eq.19
  Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);// eq.19
  
  Vector_Cross_Product(&temporary[2][0],&temporary[0][0], &temporary[1][0]); // c= a x b //eq.20
  
  renorm = 0.5 * (3 - Vector_Dot_Product(&temporary[0][0], &temporary[0][0])); // eq.21
  Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);
  
  renorm = 0.5 * (3 - Vector_Dot_Product(&temporary[1][0], &temporary[1][0])); // eq.21
  Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);
  
  renorm = 0.5 * (3 - Vector_Dot_Product(&temporary[2][0], &temporary[2][0])); // eq.21
  Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
}

// Drift correction
void Drift_correction(void)
{
  float mag_heading_x;
  float mag_heading_y;
  float errorCourse;
  
  // Compensation the Roll, Pitch and Yaw drift. 
  static float Scaled_Omega_P[3];
  static float Scaled_Omega_I[3];
  float Accel_magnitude;
  float Accel_weight;
  
  // ROLL & PITCH
  // Calculate the magnitude of the accelerometer vector
  Accel_magnitude = sqrt(Accel_Vector[0] * Accel_Vector[0] + Accel_Vector[1] * Accel_Vector[1] + Accel_Vector[2] * Accel_Vector[2]);
  Accel_magnitude = Accel_magnitude / GRAVITY; // scale to gravity
  // Dynamic weighting of accelerometer info (reliability filter)
  // Weight for accelerometer info (< 0.5G = 0.0, 1G = 1.0 , > 1.5G = 0.0)
  Accel_weight = constrain(1 - 2 * abs(1 - Accel_magnitude), 0, 1); 

  Vector_Cross_Product(&errorRollPitch[0], &Accel_Vector[0], &DCM_Matrix[2][0]); // adjust the ground of reference
  Vector_Scale(&Omega_P[0], &errorRollPitch[0], Kp_ROLLPITCH * Accel_weight);
  
  Vector_Scale(&Scaled_Omega_I[0], &errorRollPitch[0], Ki_ROLLPITCH * Accel_weight);
  Vector_Add(Omega_I, Omega_I, Scaled_Omega_I);     
  
  // YAW
  // We make the gyro YAW drift correction based on compass magnetic heading
  mag_heading_x = cos(MAG_Heading);
  mag_heading_y = sin(MAG_Heading);
  
  
  errorCourse=(DCM_Matrix[0][0] * mag_heading_y) - (DCM_Matrix[1][0] * mag_heading_x); // calculating YAW error
  Vector_Scale(errorYaw, &DCM_Matrix[2][0], errorCourse); // apply the yaw correction to the XYZ rotation of the object, depeding the position.
  
  Vector_Scale(&Scaled_Omega_P[0], &errorYaw[0], Kp_YAW); // 0.01 proportional of YAW.
  Vector_Add(Omega_P, Omega_P, Scaled_Omega_P); // add  proportional
  
  Vector_Scale(&Scaled_Omega_I[0], &errorYaw[0], Ki_YAW); // 0.00001 integrator
  Vector_Add(Omega_I, Omega_I, Scaled_Omega_I); // add integrator to the Omega_I
}

// Euler angles
void Euler_angles(void)
{
  pitch = -asin(DCM_Matrix[2][0]);
  roll = atan2(DCM_Matrix[2][1], DCM_Matrix[2][2]);
  yaw = atan2(DCM_Matrix[1][0], DCM_Matrix[0][0]);
}

