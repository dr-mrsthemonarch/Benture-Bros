/*
void loop(void)
{


  float ax = emus[3].getAccelX_mss();
  float ay = emus[3].getAccelY_mss();
  float az = emus[3].getAccelZ_mss();

  // Apply mag offset compensation (base values in uTesla)
  double x = emus[3].getMagX_uT() - mag_offsets[0];
  double yy = emus[3].getMagY_uT() - mag_offsets[1];
  double z = emus[3].getMagZ_uT() - mag_offsets[2];

  // Apply mag soft iron error compensation
  double mx = x * mag_softiron_matrix[0][0] + yy * mag_softiron_matrix[0][1] + z * mag_softiron_matrix[0][2];
  double my = x * mag_softiron_matrix[1][0] + yy * mag_softiron_matrix[1][1] + z * mag_softiron_matrix[1][2];
  double mz = x * mag_softiron_matrix[2][0] + yy * mag_softiron_matrix[2][1] + z * mag_softiron_matrix[2][2];

  // Apply gyro zero-rate error compensation

  // The filter library expects gyro data in degrees/s, but adafruit sensor
  // uses rad/s so we need to convert them first (or adapt the filter lib
  // where they are being converted)
  float gx = emus[3].getGyroX_rads() ;
  float gy =  emus[3].getGyroX_rads();
  float gz = emus[3].getGyroX_rads() ;

  //===================================================================================================================================================================


  filter.update(gx, gy, gz, ax, ay, az, mx, my,  -1 mz);

  Serial.print(filter.getPitch()); // print in degrees
  Serial.print(",");
  Serial.print(filter.getYaw());
  Serial.print(",");
  Serial.println(filter.getRoll());

delay(10);
}
*/
