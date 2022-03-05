
void loop(void)
{
  //=======================Update MPUS=============================================================================================================

  // read the sensors
  readIMU();


  // update accel, gyro, mag arrays
/*
  getAccelData(getAccel);
  getGyroData(getGyro);
  getMagData(getMag);




  // Apply magnetic offsets
  for (int j = 0; j < 6; j++) {
    for (int i = 0; i < 4; i++) {
      deltamag[j][i] = getMag[j][i] - mag_offsets[i][j];
    }
  }

  // Apply magnetic softiron offsets
  for (int k = 0; k < 6; k++) {
    for (int j = 0; j < 6; j++) {
      for (int i = 0; i < 4; i++) {
        mag[j][i] = deltamag[j][0] * mag_softiron_matrix[k][0][0] + deltamag[j][1] * mag_softiron_matrix[k][0][1] + deltamag[j][2] * mag_softiron_matrix[k][0][2];
      }
    }
  }

  // Apply gyroscope offsets
  for (int j = 0; j < 6; j++) {
    for (int i = 0; i < 4; i++) {
      gyro[j][i] = (getGyro[j][i] - gyro_zero_offsets[j][i]) * 57.2958F; // filter expects data in degs multiply 180/pi;
    }
  }


  // Update Madgwick filters with Sparkfun MPU9250, magZ must be inverted, check/test Bolder Library for transformation matrix tX[3],tY[3],tZ[3],
  // "transform the accel and gyro axes to match the magnetometer axes" Fix first if angles are inverted/rotated compared to magn.

  for (int j = 0; j < 6; j++) {
    filters[j].update(gyro[j][0], gyro[j][1], gyro[j][2], accel[j][0], accel[j][1], accel[j][2], mag[j][0], mag[j][1], -1 * mag[j][2]);
  }

  // Call All Euler Angle Rotations around {X,Y,Z} or {gamma, delta, epsilon}

  updateEulerAngles(eulerAngles);
*/

  //=======================Update Motor angular velocities=============================================================================================================

  // Read TI ADS1015
  for (int i = 0; i < 2; i++) {
    adc[i] = readADC(i);
  }
  adc[2] = readADC(3); // ADC 2 is dead on the logic board using ADCnumber 3

  //Update Omega Array
  for (int i = 0; i < 3; i++) {
    omega[i] = pwmtoRPM(adc[i]);
  }


  //=============================Read Serial and Print data========================================================================================================

  //read serial for manual control
  if (Serial.available() > 0) {
    stringRead = Serial.readStringUntil('\n');
  }
  switcher = getValue(stringRead, ':', 0);
  value = getValue(stringRead, ':', 1);
  value1 = getValue(stringRead, ':', 2);
  chipselect = getValue(stringRead, ':', 0);

  manual(switcher.toInt(), value, value1);

  //=============================Read Serial and Print data========================================================================================================
  /*
    Serial.print(eulerAngles[0][0]);
    Serial.print(',');
    Serial.print(eulerAngles[0][1]);
    Serial.print(',');
    Serial.print(eulerAngles[0][2]);
    Serial.print(',');
    Serial.print(eulerAngles[3][0]);
    Serial.print(',');
    Serial.print(eulerAngles[3][1]);
    Serial.print(',');
    Serial.println(eulerAngles[3][2]);
  */

  Serial.print(omega[0]);
  Serial.print(',');
  Serial.print(omega[1]);
  Serial.print(',');
  Serial.println(omega[2]);


  // delay system for debugging.
  delay(10);
}
