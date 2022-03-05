
// Init filter
Madgwick filters[6];
Madgwick filter;

// Set SPI channel and CS pins for IMU communication
int cs[6] = {21, 25, 26, 27, 32, 14};

MPU9250 emus[6] = {MPU9250(SPI, 21), MPU9250(SPI, 25), MPU9250(SPI, 26), MPU9250(SPI, 27), MPU9250(SPI, 32), MPU9250(SPI, 14)};

int imuStatus[6]; // IMU status array



  // Offsets applied to raw x/y/z mag values
  float mag_offsets[6][3] = {
  // IMUs, 21, 25, 26
  { 10.54F, 35.93F, -55.04F },
  { 0, 0, 0 },
  { 0, 0, 0 },
  // IMUs 27, 32, 14
  { -3.47F, 17.1F, -11.11F },
  { 0, 0, 0 },
  { 0, 0, 0 },
  };

  // Soft iron error compensation matrix
  float mag_softiron_matrix[6][3][3] = {
  // IMUs, 21, 25, 26
  {{  0.960F,  -0.019F,  -0.024F }, {  -0.019F,  1.062F, 0.030F }, {  -0.024F, 0.030F,  0.982F }},
  {{  0, 0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  {{  0, 0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  // IMUs 27, 14, 32
  {{  0.941F,  -0.059F,  -0.012F }, {  -0.059F,  1.070F, -0.011F }, {  -0.012F, -0.011F,  0.997F }},
  {{  0, 0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  {{  0, 0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  };

  float mag_field_strength[6] = {41.85F,  0 , 0, 37.61, 0, 0 };

/*
// Offsets applied to raw x/y/z mag values
float mag_offsets[3]            = { -3.47F, 17.1F, -11.11F };
// Soft iron error compensation matrix
float mag_softiron_matrix[3][3] = {
  {  0.941,  -0.059,  -0.012 },
  {  -0.059,  1.070, -0.011 },
  {  -0.012, -0.011,  0.997 }
};
float mag_field_strength        = 37.61F;
// Offsets applied to compensate for gyro zero-drift error for x/y/z
*/



// Offsets applied to compensate for gyro zero-drift error for x/y/z, sensor dependent
float gyro_zero_offsets[6][3] = {
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
};

// Used for calculating 'in between values' prior to passing to final mag array, sensor dependent
float deltamag[6][3] = {
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
};

// Following array names should always be constant and final values to be given to Magdwick filters, sensor agnostic.
float gyro[6][3] = {
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
};

float accel[6][3] = {
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
};

float mag[6][3] = {
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
};

// update accel, gyro, mag arrays
float getAccel[6][3] = {0};

float getGyro[6][3] = {0};

float getMag[6][3] = {0};

float eulerAngles[6][3] = {0};
