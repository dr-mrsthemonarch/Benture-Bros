
// Only Helper Functions
void schedularOnOff(int sensorSelect) {
  if (sensorSelect < 6) {
    switch (sensorSelect) {
      case 3:
        runner.addTask(cs14_RPY);
        angleCalculateRPY_Rads.enable();
        cs14_RPY.enable();
        break;
      case 4:
        cs14_RPY.disable();
        angleCalculateRPY_Rads.disable();
        runner.deleteTask(cs14_RPY);
        break;
      case 5:
        blinker.disable();
        LED_state = false;
        LEDOff();
        runner.addTask(converger);
        converger.enable();
        break;
    }
  }
}

inline void LEDOn() {
  digitalWrite( LED_BUILTIN, HIGH );
}

inline void LEDOff() {
  digitalWrite( LED_BUILTIN, LOW );
}
inline void enableMotor() {
  digitalWrite(enablePin, HIGH);
}
inline void disableMotor() {
  digitalWrite(enablePin, LOW);
}

void parser() {
  // For use with SimpleCLI
  if (cli.available()) {
    // Get command out of queue
    Command cmd = cli.getCmd();
  }
}
float pwmToRPM(float voltage) {
  // calculate RPM from range of 0.1v to 3v 12 bit
  float RPM = (8.53108 * voltage - 6594.5);
  float rads = (0.1047197551 * RPM); // convert to rad*s^(-1)
  return rads;
}


float ampsToPWM(float sgnl) {
  // convert Amps to PWM signal
  float pwmVoltage;
  pwmVoltage = (((highPWM) - (lowPWM)) / 4.4) * sgnl + nullPWM;
  return pwmVoltage;
}

int readADC(int channel) {
  //  return ads.readADC_SingleEnded(channel);
  return channel;
}

void startLEDC() {
  for (int i = 0; i < 3; i++) {
    // configure LED PWM functionalitites
    ledcSetup(ledChannel[i], freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(motorPin[i], ledChannel[i]);

    // escon requires 10% PWM signal to activate, set to nullPWM
    ledcWrite(ledChannel[i], nullPWM);
  }
}
