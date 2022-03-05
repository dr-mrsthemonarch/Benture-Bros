String getValue(String data, char separator, float index)
// read multiple values from string with : delimiter
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


float pwmtoRPM(float pwm) {
  // calculate RPM from range of 0.1v to 3v 12 bit
  float RPM = (8.53108 * pwm - 6594.5);
  float rads = (0.1047197551 * RPM); // convert to rad*s^(-1)
  return rads;
}


float ampsToPWM(float sgnl) {
  // convert Amps to PWM signal
  float thing;
  thing = (((highPWM) - (lowPWM)) / 4.4) * sgnl + nullPWM;
  return thing;
}


void motorControl(float sgnl, int16_t channel) {
  //Set motor current input
  float pwm = ampsToPWM(sgnl);
  ledcWrite(channel, pwm);
}

int readADC(int channel) {
  return ads.readADC_SingleEnded(channel);
}

void dataprint(float list[6][3], int channel) {
  Serial.print(list[channel][0]); // print in degrees
  Serial.print(",");
  Serial.print(list[channel][1]); // print in degrees
  Serial.print(",");
  Serial.println(list[channel][2]); // print in degrees
}

void readIMU() {
  for (int j = 0; j < 6; j++) {
    emus[j].readSensor();
  }
}

void updateEulerAngles(float list[6][3]) {
  for (int j = 0; j < 6; j++) {
    list[j][0] = filters[j].getRoll();
    list[j][1] = filters[j].getPitch();
    list[j][2] = filters[j].getYaw();
  }
};


void getAccelData(float list[6][3]) {
  for (int j = 0; j < 6; j++) {
    list[j][0] = emus[j].getAccelX_mss();
    list[j][1] = emus[j].getAccelY_mss();
    list[j][2] = emus[j].getAccelZ_mss();
  }
}


void getGyroData(float list[6][3]) {
  for (int j = 0; j < 6; j++) {
    list[j][0] = emus[j].getGyroX_rads() ; // filter expects data in degs multiply 180/pi
    list[j][1] = emus[j].getGyroY_rads() ;
    list[j][2] = emus[j].getGyroZ_rads() ;
  }
}

void getMagData(float list[6][3]) {
  for (int j = 0; j < 6; j++) {
    list[j][0] = emus[j].getMagX_uT();
    list[j][1] = emus[j].getMagY_uT();
    list[j][2] = emus[j].getMagZ_uT();
  }
}


// function multiplies all elements of 3x3 matrix A by the specified scalar
void f3x3matrixAeqAxScalar(float A[][3], float Scalar)
{
  float *pAij;  // pointer to A[i][j]
  int8_t i, j;  // loop counters

  for (i = 0; i < 3; i++) {
    // set pAij to &A[i][j=0]
    pAij = A[i];
    for (j = 0; j < 3; j++) {
      *(pAij++) *= Scalar;
    }
  }
}
