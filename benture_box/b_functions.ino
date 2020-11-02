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


void motor(float lqrSignal, int16_t channel) {
  //Set motor current input
  float pwm = ampsToPWM(lqrSignal);
  ledcWrite(channel, pwm);
}
