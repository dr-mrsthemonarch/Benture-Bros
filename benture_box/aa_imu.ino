
// Init filter
Madgwick filter0;
Madgwick filter1;
Madgwick filter2;
Madgwick filter3;
Madgwick filter4;
Madgwick filter5;

// Set SPI channel and CS pins for IMU communication
int cs[6] = {27, 14, 32, 21, 25, 26};

MPU9250 IMU0(SPI, cs[0]);
MPU9250 IMU1(SPI, cs[1]);
MPU9250 IMU2(SPI, cs[2]);
MPU9250 IMU3(SPI, cs[3]);
MPU9250 IMU4(SPI, cs[4]);
MPU9250 IMU5(SPI, cs[5]);

int imuStatus[6]; // IMU status array


// Offsets applied to raw x/y/z mag values
float mag_offsets[6][3] = {
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 0.0F, 0.0F, 0.0F },
  { 10.44F, 34.76F, -49.86F },
  { 8.62F, 20.41F, -12.65F },
  { -3.05F, 19.75F, -8.55F },
};

// Soft iron error compensation matrix
float mag_softiron_matrix[6][3][3] = {
  // IMUs 27, 14, 32
  {{  0,  0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  {{  0,  0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  {{  0,  0,  0 }, {  0,  0,  0 }, {  0,  0,  0 }},
  // IMUs, 21, 25,26
  {{  1.036F,  0.017F,  -0.001F }, {  0.017F,  0.954F, -0.028F }, {  -0.001F, 0.028F,  1.013F }},
  {{  1.031F,  0.013F,  -0.024F }, {  0.013F,  0.897F,  0.054F }, {  -0.024F,  0.054F,  1.085F }},
  {{  1.057F,  0.034F,  0.017F }, {  0.034F,  0.967F,  0.038F }, {  0.017F,  0.038F,  0.981F }},
};

float mag_field_strength[3] = {38.52F, 37.24F , 38.58F };

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
