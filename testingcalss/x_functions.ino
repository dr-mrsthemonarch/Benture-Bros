
inline void LEDOn()
{
  digitalWrite(LED_BUILTIN, HIGH);
}

inline void LEDOff()
{
  digitalWrite(LED_BUILTIN, LOW);
}

float sensorOffset(float gain, float sensor1, float sensor2)
{
  float sol;
  sol = gain * sensor2 + sensor1;
  return sol;
}

void parser()
{
  // For use with SimpleCLI
  if (cli.available())
  {
    // Get command out of queue
    Command cmd = cli.getCmd();
  }
}

void taskSensorOffset()
{
  for (int i = 0; i < 3; i++)
  {
    phiRPY[i] = sensorOffset(-1, rpyValues[0][i], rpyValues[1][i]); // angle phi is equal to gain times sensor 1 angle [i] subtract sensor sensor 2 angle [i]
    thetaRPY[i] = sensorOffset(-1, rpyValues[2][i], rpyValues[3][i]); //not required for breadboardtestbench
    psiRPY[i] = sensorOffset(-1, rpyValues[4][i], rpyValues[5][i]);
  }
}

float updateLQR(Vector3f &lqr, const Vector3f &plant)
{
  float sol;
  sol = lqr.dot(plant);
  return sol;
}

inline void enableMotor()
{
  digitalWrite(enablePin, HIGH);
}
inline void disableMotor()
{
  digitalWrite(enablePin, LOW);
}

void motorControl(float sgnl, int16_t channel)
{
  // Set motor current input
  float pwm = ampsToPWM(sgnl);
  ledcWrite(channel, pwm);
}

float pwmToRads(float voltage)
//used for printing 
{
  // calculate RPM from range of 0.1v to 3v 12 bit
  float RPM = (8.53108 * voltage - 6594.5);
  float rads = (0.1047197551 * RPM); // convert to rad*s^(-1)
  return rads;
}

float ampsToPWM(float sgnl)
//used for motor control
{
  // convert Amps to PWM signal
  float pwmVoltage;
  pwmVoltage = (((highPWM) - (lowPWM)) / 4.4) * sgnl + nullPWM;
  return pwmVoltage;
}

int readADC(int channel) {
  return ads.readADC_SingleEnded(channel);
}

void startLEDC()
//required for starting pwm timers
{
  for (int i = 0; i < 3; i++)
  {
    // configure LED PWM functionalitites
    ledcSetup(ledChannel[i], freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(motorPin[i], ledChannel[i]);

    // escon requires 10% PWM signal to activate, set to nullPWM
    ledcWrite(ledChannel[i], nullPWM);
  }
}