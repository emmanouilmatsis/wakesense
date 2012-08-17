
/* -------- Correlation coefficient -------- */

// Initialise distributions
void PCC_Init() {
  for(int i=0; i<PCC_SAMPLES; i++) {
    // Initialise random distribution
    random_distribution[i] = random(-7, 7);
    
    // Initialise sample distribution
    read_sensors(); // read sensor data
    sample_distribution[i] = sqrt(accel[0] * accel[0] + accel[1] * accel[1] + accel[2] * accel[2]) / GRAVITY; // combined axis g
    delay(20); // give sensors enough time to collect data
  }
}

// Update sample distribution
void PCC_Update() {
  for(int i=PCC_SAMPLES - 1; i>-1; i--) {
   sample_distribution[i - 1] = sample_distribution[i]; 
  }
  sample_distribution[PCC_SAMPLES - 1] = sqrt(accel[0] * accel[0] + accel[1] * accel[1] + accel[2] * accel[2]) / GRAVITY; // combined axis g
}

// Pearsons correlation coefficient
float PCC() {
  // Sum
  float sumX = sum(random_distribution, PCC_SAMPLES);
  float sumY = sum(sample_distribution, PCC_SAMPLES);
  
  // Square sum
  float sumXSq = sumSquare(random_distribution, PCC_SAMPLES);
  float sumYSq = sumSquare(sample_distribution, PCC_SAMPLES);
  
  // Product sum
  float sumPr = sumProduct(random_distribution, sample_distribution, PCC_SAMPLES);
  
  // Correlation coefficient 
  float num = sumPr - (sumX * sumY / PCC_SAMPLES);
  float den = sqrt((( sumXSq - pow(sumX, 2) / PCC_SAMPLES) * (sumYSq - pow(sumY, 2) / PCC_SAMPLES)));
    
  // Return
  if(den == 0) {
    return 0;
  } else {
    return num / den;
  }
}

// Sum of array elements
float sum(float* arr, int len) {
  float result = 0;
  for(int i=0; i<len; i++) {
    result += arr[i];
  }
  return result;
}

// Sum of squared array elements
float sumSquare(float* arr, int len) {
  float result = 0;
  for(int i=0; i<len; i++) {
    result += pow(arr[i], 2);
  }
  return result;
}

// Sum of products
float sumProduct(float* arr1, float* arr2, int len) {
  float result = 0;
  for(int i=0; i<len; i++) {
    result += arr1[i] * arr2[i];
  }
  return result;
}
