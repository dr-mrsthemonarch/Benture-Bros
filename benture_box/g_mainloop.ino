void loop(void)
{
  //=======================Update MPUS=============================================================================================================
  // read the sensors
  IMU0.readSensor();
  IMU1.readSensor();
  IMU2.readSensor();
  //  IMU3.readSensor();
  // IMU4.readSensor();
  // IMU5.readSensor();

  // update accel, gyro, mag arrays
  float accel[3][3] = {
    {IMU0.getAccelX_mss(), IMU0.getAccelY_mss(), IMU0.getAccelZ_mss()},
    {IMU1.getAccelX_mss(), IMU1.getAccelY_mss(), IMU1.getAccelZ_mss()},
    {IMU2.getAccelX_mss(), IMU2.getAccelY_mss(), IMU2.getAccelZ_mss()},
    //    {IMU3.getAccelX_mss(), IMU3.getAccelY_mss(), IMU3.getAccelZ_mss()},
    //    {IMU4.getAccelX_mss(), IMU4.getAccelY_mss(), IMU4.getAccelZ_mss()},
    //    {IMU5.getAccelX_mss(), IMU5.getAccelY_mss(), IMU5.getAccelZ_mss()},
  };
  float gyro[3][3] = {
    {IMU0.getGyroX_rads(), IMU0.getGyroY_rads(), IMU0.getGyroZ_rads()},
    {IMU1.getGyroX_rads(), IMU1.getGyroY_rads(), IMU1.getGyroZ_rads()},
    {IMU2.getGyroX_rads(), IMU2.getGyroY_rads(), IMU2.getGyroZ_rads()},
    //    {IMU3.getGyroX_rads(), IMU3.getGyroY_rads(), IMU3.getGyroZ_rads()},
    //    {IMU4.getGyroX_rads(), IMU4.getGyroY_rads(), IMU4.getGyroZ_rads()},
    //    {IMU5.getGyroX_rads(), IMU5.getGyroY_rads(), IMU5.getGyroZ_rads()},
  };
  float mag[3][3] = {
    {IMU0.getMagX_uT(), IMU0.getMagY_uT(), IMU0.getMagZ_uT()},
    {IMU1.getMagX_uT(), IMU1.getMagY_uT(), IMU1.getMagZ_uT()},
    {IMU2.getMagX_uT(), IMU2.getMagY_uT(), IMU2.getMagZ_uT()},
    //    {IMU3.getMagX_uT(), IMU3.getMagY_uT(), IMU3.getMagZ_uT()},
    //    {IMU4.getMagX_uT(), IMU4.getMagY_uT(), IMU4.getMagZ_uT()},
    //    {IMU5.getMagX_uT(), IMU5.getMagY_uT(), IMU5.getMagZ_uT()},
  };


  /*
      // Madgwick lib requires gyro data in Degrees per sec. convert
      for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
          gyro[j][i] *= 57.2958;
        }
      }
  */

  // Update Madgwick filters with Sparkfun MPU9250, magZ must be inverted.
  filter0.update(gyro[0][0], gyro[0][1], gyro[0][2], accel[0][0], accel[0][1], accel[0][2], mag[0][0], mag[0][1], -1 * mag[0][2]);
  filter1.update(gyro[1][0], gyro[1][1], gyro[1][2], accel[1][0], accel[1][1], accel[1][2], mag[1][0], mag[1][1], -1 * mag[1][2]);
  filter2.update(gyro[2][0], gyro[2][1], gyro[2][2], accel[2][0], accel[2][1], accel[2][2], mag[2][0], mag[2][1], -1 * mag[2][2]);
  // filter3.update(gyro[3][0], gyro[3][1], gyro[3][2], accel[3][0], accel[3][1], accel[3][2], mag[3][0], mag[3][1], -1 * mag[3][2]);
  // filter4.update(gyro[4][0], gyro[4][1], gyro[4][2], accel[4][0], accel[4][1], accel[4][2], mag[4][0], mag[4][1], -1 * mag[4][2]);
  // filter5.update(gyro[5][0], gyro[5][1], gyro[5][2], accel[5][0], accel[5][1], accel[5][2], mag[5][0], mag[5][1], -1 * mag[5][2]);
  // filternxp.update(gyro[5][0], gyro[5][1], gyro[5][2], accel[5][0], accel[5][1], accel[5][2], mag[5][0], mag[5][1], -1 * mag[5][2]);

  // Call All Euler Angle Rotations around {X,Y,Z} or {gamma, delta, epsilon}
  float eulerAngles[3][3] =  {
    {filter0.getRoll(), filter0.getPitch(),  filter0.getYaw()},
    {filter1.getRoll(), filter1.getPitch(),  filter1.getYaw()},
    {filter2.getRoll(), filter2.getPitch(),  filter2.getYaw()},
    //    {filter3.getRoll(), filter3.getPitch(),  filter3.getYaw()},
    //    {filter4.getRoll(), filter4.getPitch(),  filter4.getYaw()},
    //    {filter5.getRoll(), filter5.getPitch(),  filter5.getYaw()},
  };
  float qw, qx, qy, qz;
  filter0.getQuaternion(&qw, &qx, &qy, &qz);

  //=======================Update Motor angular velocities=============================================================================================================

  // Read TI ADS1015
  adc[0] = ads.readADC_SingleEnded(0);
  //  adc[1] = ads.readADC_SingleEnded(1);
  //  adc[2] = ads.readADC_SingleEnded(2);
  //Update Omega Array
  omega[0] = pwmtoRPM(adc[0]);
  //  omega[1] = pwmtoRPM(adc[1]);
  //  omega[2] = pwmtoRPM(adc[2]);

  //=============================Read Serial and Print data========================================================================================================

  //read serial for manual control
  if (Serial.available() > 0) {
    stringRead = Serial.readString();
  }
  enable = getValue(stringRead, ':', 0);
  v1 = getValue(stringRead, ':', 1);
  v2 = getValue(stringRead, ':', 2);
  v3 = getValue(stringRead, ':', 3);
  manualControl = enable.toInt();
  manual(manualControl, v1, v2, v3);



  //Print Data to Serial

  Serial.print(eulerAngles[0][0]); // print in degrees
  Serial.print(",");
  Serial.print(eulerAngles[0][1]); // print in degrees
  Serial.print(",");
  Serial.println(eulerAngles[0][2]); // print in degrees

  /*
    Serial.print(xx); // print in degrees
    Serial.print(",");
    Serial.print(yy); // print in degrees
    Serial.print(",");
    Serial.println(zz); // print in degrees
  */
  // delay system for debugging.
  delay(10);
}
