
// Init filter
Adafruit_Madgwick filter0;
Adafruit_Madgwick filter1;
Adafruit_Madgwick filter2;
Adafruit_Madgwick filter3;
Adafruit_Madgwick filter4;
Adafruit_Madgwick filter5;

// Set SPI channel and CS pins
int cs[6] = {27,14,21,32,25,26};

MPU9250 IMU0(SPI, cs[0]);
MPU9250 IMU1(SPI, cs[1]);
MPU9250 IMU2(SPI, cs[2]);
MPU9250 IMU3(SPI, cs[3]);
MPU9250 IMU4(SPI, cs[4]);
MPU9250 IMU5(SPI, cs[5]);

int imuStatus[6]; // IMU status array
