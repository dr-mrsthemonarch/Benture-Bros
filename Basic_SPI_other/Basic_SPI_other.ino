#include <Adafruit_AHRS.h>

#include "MPU9250.h"


Adafruit_Madgwick filter1;
Adafruit_Madgwick filter2;
Adafruit_Madgwick filter3;
MPU9250 IMU1(SPI, 14);
MPU9250 IMU2(SPI, 26);
MPU9250 IMU3(SPI, 25);
int status1, status2, status3;

void setup() {
  // serial to display data
  Serial.begin(250000);
  while (!Serial) {}
  // start the mpus
  status1 = IMU1.begin();
  status2 = IMU2.begin();
  status3 = IMU3.begin();
  // start communication with IMU
  IMU1.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU1.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU2.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU2.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU3.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU3.setGyroRange(MPU9250::GYRO_RANGE_250DPS);

  status1 = IMU1.calibrateAccel();
  status2 = IMU2.calibrateAccel();
  status3 = IMU3.calibrateAccel();

  delay(200);
  /*
    if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    //    Serial.println(status);
    while (1) {}
    }
  */
  filter1.begin(300);
  filter2.begin(300);
  filter3.begin(300);
}

float ax1, ay1, az1, gx1, gy1, gz1, mx1, my1, mz1;
float ax2, ay2, az2, gx2, gy2, gz2, mx2, my2, mz2;
float ax3, ay3, az3, gx3, gy3, gz3, mx3, my3, mz3;
void loop() {
  // read the sensor
  IMU1.readSensor();
  IMU2.readSensor();
  IMU3.readSensor();
  // caution axis ax, ay, az are rotated 90 degrees
  ax1 = IMU1.getAccelX_mss();
  ay1 = IMU1.getAccelY_mss();
  az1 = IMU1.getAccelZ_mss();
  gx1 = IMU1.getGyroX_rads();
  gy1 = IMU1.getGyroY_rads();
  gz1 = IMU1.getGyroZ_rads();
  mx1 = IMU1.getMagX_uT();
  my1 = IMU1.getMagY_uT();
  mz1 = IMU1.getMagZ_uT();

  ax2 = IMU2.getAccelX_mss();
  ay2 = IMU2.getAccelY_mss();
  az2 = IMU2.getAccelZ_mss();
  gx2 = IMU2.getGyroX_rads();
  gy2 = IMU2.getGyroY_rads();
  gz2 = IMU2.getGyroZ_rads();
  mx2 = IMU2.getMagX_uT();
  my2 = IMU2.getMagY_uT();
  mz2 = IMU2.getMagZ_uT();

  ax3 = IMU3.getAccelX_mss();
  ay3 = IMU3.getAccelY_mss();
  az3 = IMU3.getAccelZ_mss();
  gx3 = IMU3.getGyroX_rads();
  gy3 = IMU3.getGyroY_rads();
  gz3 = IMU3.getGyroZ_rads();
  mx3 = IMU3.getMagX_uT();
  my3 = IMU3.getMagY_uT();
  mz3 = IMU3.getMagZ_uT();

  gx1 *= 57.2958F;
  gy1 *= 57.2958F;
  gz1 *= 57.2958F;
  gx2 *= 57.2958F;
  gy2 *= 57.2958F;
  gz2 *= 57.2958F;
  gx3 *= 57.2958F;
  gy3 *= 57.2958F;
  gz3 *= 57.2958F;
  filter1.update(gx1, gy1, gz1, ax1, ay1, az1, mx1, my1, mz1);
  filter2.update(gx2, gy2, gz2, ax2, ay2, az2, mx2, my2, mz2);
  filter3.update(gx3, gy3, gz3, ax3, ay3, az3, mx3, my3, mz3);
  float theta1 = filter1.getPitch();
  float theta2 = filter2.getPitch();
  float theta3 = filter3.getPitch();

  Serial.print(theta1, 2);
  Serial.print(",");
  Serial.print(theta2, 2);
  Serial.print(",");
  Serial.println(theta3, 2);
}
