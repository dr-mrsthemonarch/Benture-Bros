/*
// Used for mag, accel,gryo, motor calibration, consider changing for state machine

void loop(void)
{
  //=======================Update MPUS=============================================================================================================
  // read the sensors
  IMU0.readSensor();
  IMU1.readSensor();
  IMU2.readSensor();
  IMU3.readSensor();
  IMU4.readSensor();
  IMU5.readSensor();

  // update accel, gyro, mag arrays
  int accel[6][3] = {
    {IMU0.getAccelX_mss(), IMU0.getAccelY_mss(), IMU0.getAccelZ_mss()},
    {IMU1.getAccelX_mss(), IMU1.getAccelY_mss(), IMU1.getAccelZ_mss()},
    {IMU2.getAccelX_mss(), IMU2.getAccelY_mss(), IMU2.getAccelZ_mss()},
    {IMU3.getAccelX_mss(), IMU3.getAccelY_mss(), IMU3.getAccelZ_mss()},
    {IMU4.getAccelX_mss(), IMU4.getAccelY_mss(), IMU4.getAccelZ_mss()},
    {IMU5.getAccelX_mss(), IMU5.getAccelY_mss(), IMU5.getAccelZ_mss()},
  };
  int gyro[6][3] = {
    {IMU0.getGyroX_rads(), IMU0.getGyroY_rads(), IMU0.getGyroZ_rads()},
    {IMU1.getGyroX_rads(), IMU1.getGyroY_rads(), IMU1.getGyroZ_rads()},
    {IMU2.getGyroX_rads(), IMU2.getGyroY_rads(), IMU2.getGyroZ_rads()},
    {IMU3.getGyroX_rads(), IMU3.getGyroY_rads(), IMU3.getGyroZ_rads()},
    {IMU4.getGyroX_rads(), IMU4.getGyroY_rads(), IMU4.getGyroZ_rads()},
    {IMU5.getGyroX_rads(), IMU5.getGyroY_rads(), IMU5.getGyroZ_rads()},
  };
  int mag[6][3] = {
    {IMU0.getMagX_uT(), IMU0.getMagY_uT(), IMU0.getMagZ_uT()},
    {IMU1.getMagX_uT(), IMU1.getMagY_uT(), IMU1.getMagZ_uT()},
    {IMU2.getMagX_uT(), IMU2.getMagY_uT(), IMU2.getMagZ_uT()},
    {IMU3.getMagX_uT(), IMU3.getMagY_uT(), IMU3.getMagZ_uT()},
    {IMU4.getMagX_uT(), IMU4.getMagY_uT(), IMU4.getMagZ_uT()},
    {IMU5.getMagX_uT(), IMU5.getMagY_uT(), IMU5.getMagZ_uT()},
  };

  //=======================Update Motor angular velocities=============================================================================================================


  //=============================Read Serial and Print data========================================================================================================


  //Print Data to Serial

  Serial.print("Raw:");
  Serial.print(accel[3][0]);
  Serial.print(',');
  Serial.print(accel[3][1]);
  Serial.print(',');
  Serial.print(accel[3][2]);
  Serial.print(',');
  Serial.print(gyro[3][0]);
  Serial.print(',');
  Serial.print(gyro[3][1]);
  Serial.print(',');
  Serial.print(gyro[3][2]);
  Serial.print(',');
  Serial.print(mag[5][0]*10);
  Serial.print(',');
  Serial.print(mag[5][1]*10);
  Serial.print(',');
  Serial.print(mag[5][2]*10);
  Serial.println();
  // delay system for debugging.
  // delay(10);
}

*/
