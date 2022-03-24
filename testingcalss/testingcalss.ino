#include "header.hpp"

// Prototypes for tasks
//========================== Accel Raw
void cs26PrintAccel_Raw();
void cs14PrintAccel_Raw();
void cs21PrintAccel_Raw();
void cs27PrintAccel_Raw();
void cs32PrintAccel_Raw();
void cs25PrintAccel_Raw();
void bnoPrintAccel_Raw();
//========================== Accel Mss
void cs26PrintAccel_Mss();
void cs14PrintAccel_Mss();
void cs21PrintAccel_Mss();
void cs27PrintAccel_Mss();
void cs32PrintAccel_Mss();
void cs25PrintAccel_Mss();
void bnoPrintAccel_Mss();
//========================== RPY Rads
void cs26Print_Rads();
void cs14Print_Rads();
void cs21Print_Rads();
void cs27Print_Rads();
void cs32Print_Rads();
void cs25Print_Rads();
void bnoPrint_Rads();
//========================== Marg
void cs26PrintMarg();
void cs14PrintMarg();
void cs21PrintMarg();
void cs27PrintMarg();
void cs32PrintMarg();
void cs25PrintMarg();
//========================== Gyro
void cs26PrintGyro();
void cs14PrintGyro();
void cs21PrintGyro();
void cs27PrintGyro();
void cs32PrintGyro();
void cs25PrintGyro();
void bnoPrintGyro();
//========================== Print Plants
void plantPrintAll();
void plantPrintPhi();
void plantPrintTheta();
void plantPrintPsi();
void plantPrintLambda();

void taskPrintU();

void blinkCB();
void taskConverge();
void readyLED();
void taskCalculateRPY_Rads();
void taskCalculateEuler();
void taskReadSerial();
void taskUpdateLQRAll();
void taskUpdateLQRPhi();
void taskUpdateLQRTheta();
void taskUpdateLQRPsi();
void taskUpdatePlant();
void taskGetOmega();
void taskCurrentControllerAll();
void taskCurrentControllerPhi();
void taskCurrentControllerTheta();
void taskCurrentControllerPsi();
void taskPrintDebug();
void taskPrintOmega();
void taskUpdateFullState();

// Tasks to always run
Task serialRead(20, TASK_FOREVER, &taskReadSerial);
Task converger(11, TASK_FOREVER, &taskConverge);
Task blinker(500, TASK_FOREVER, &blinkCB);
Task probeADC(5, TASK_FOREVER, &taskGetOmega);
Task angleCalculateRPY_Rads(1, TASK_FOREVER, &taskCalculateRPY_Rads);
Task angleCalculateEuler(1, TASK_FOREVER, &taskCalculateEuler);
Task controller(5, TASK_FOREVER, &taskUpdateLQRAll);
Task plantCalculate(1, TASK_FOREVER, &taskUpdatePlant);
Task currentControllerAll(4, TASK_FOREVER, &taskCurrentControllerAll);
Task currentControllerPhi(4, TASK_FOREVER, &taskCurrentControllerPhi);
Task currentControllerTheta(4, TASK_FOREVER, &taskCurrentControllerTheta);
Task currentControllerPsi(4, TASK_FOREVER, &taskCurrentControllerPsi);
Task doAll(2, TASK_FOREVER, &taskUpdateFullState);

// Dynamic Tasks
Task sensorReady(50, 50, &readyLED);

Task cs26_RPY(20, TASK_FOREVER, &cs26Print_Rads);
Task cs14_RPY(20, TASK_FOREVER, &cs14Print_Rads);
Task cs21_RPY(20, TASK_FOREVER, &cs21Print_Rads);
Task cs27_RPY(20, TASK_FOREVER, &cs27Print_Rads);
Task cs32_RPY(20, TASK_FOREVER, &cs32Print_Rads);
Task cs25_RPY(20, TASK_FOREVER, &cs25Print_Rads);
Task bno_RPY(20, TASK_FOREVER, &bnoPrint_Rads);

Task cs26_Accel_Raw(10, TASK_FOREVER, &cs26PrintAccel_Raw);
Task cs14_Accel_Raw(10, TASK_FOREVER, &cs14PrintAccel_Raw);
Task cs21_Accel_Raw(10, TASK_FOREVER, &cs21PrintAccel_Raw);
Task cs27_Accel_Raw(10, TASK_FOREVER, &cs27PrintAccel_Raw);
Task cs32_Accel_Raw(10, TASK_FOREVER, &cs32PrintAccel_Raw);
Task cs25_Accel_Raw(10, TASK_FOREVER, &cs25PrintAccel_Raw);
Task bno_Accel_Raw(10, TASK_FOREVER, &bnoPrintAccel_Raw);

Task cs26_Accel_Mss(10, TASK_FOREVER, &cs26PrintAccel_Mss); // 100hz for relatively good statistic
Task cs14_Accel_Mss(10, TASK_FOREVER, &cs14PrintAccel_Mss);
Task cs21_Accel_Mss(10, TASK_FOREVER, &cs21PrintAccel_Mss);
Task cs27_Accel_Mss(10, TASK_FOREVER, &cs27PrintAccel_Mss);
Task cs32_Accel_Mss(10, TASK_FOREVER, &cs32PrintAccel_Mss);
Task cs25_Accel_Mss(10, TASK_FOREVER, &cs25PrintAccel_Mss);
Task bno_Accel_Mss(10, TASK_FOREVER, &bnoPrintAccel_Mss);

Task cs26_Marg(50, TASK_FOREVER, &cs26PrintMarg); // 20hz required for motioncal
Task cs14_Marg(50, TASK_FOREVER, &cs14PrintMarg);
Task cs21_Marg(50, TASK_FOREVER, &cs21PrintMarg);
Task cs27_Marg(50, TASK_FOREVER, &cs27PrintMarg);
Task cs32_Marg(50, TASK_FOREVER, &cs32PrintMarg);
Task cs25_Marg(50, TASK_FOREVER, &cs25PrintMarg);

Task cs26_Gyro(20, TASK_FOREVER, &cs26PrintGyro);
Task cs14_Gyro(20, TASK_FOREVER, &cs14PrintGyro);
Task cs21_Gyro(20, TASK_FOREVER, &cs21PrintGyro);
Task cs27_Gyro(20, TASK_FOREVER, &cs27PrintGyro);
Task cs32_Gyro(20, TASK_FOREVER, &cs32PrintGyro);
Task cs25_Gyro(20, TASK_FOREVER, &cs25PrintGyro);
Task bno_Gyro(20, TASK_FOREVER, &bnoPrintGyro);

Task printOmega(50, TASK_FOREVER, &taskPrintOmega);
Task printPhi(20, TASK_FOREVER, &plantPrintPhi);
Task printTheta(20, TASK_FOREVER, &plantPrintTheta);
Task printPsi(20, TASK_FOREVER, &plantPrintPsi);
Task printLambda(20, TASK_FOREVER, &plantPrintLambda);
Task printPhiThetaPsiLambda(10, TASK_FOREVER, &plantPrintAll);
Task printDebug(10, TASK_FOREVER, &taskPrintDebug);

Task printU(50, TASK_FOREVER, &taskPrintU);

Scheduler runner;

//=========== Task Functions ===========
void taskReadSerial()
{
  if (Serial.available() > 0)
  {
    stringRead = Serial.readStringUntil('\n');
    cli.parse(stringRead);
  }
  parser();
}

void taskPrintU()
{
  Serial.print(u[0], 4);
  Serial.print(',');
  Serial.print(u[1], 4);
  Serial.print(',');
  Serial.println(u[2], 4);
}

void taskPrintPlant(const Vector3f &plant)
// vector indexing vector(i)
{
  Serial.print(plant(0), 4);
  Serial.print(',');
  Serial.print(plant(1), 4);
  Serial.print(',');
  Serial.println(plant(2), 4);
}

void taskPrint_Rads(int pin)
{
  Serial.print(rpyValues[pin][0], 4);
  Serial.print(',');
  Serial.print(rpyValues[pin][1], 4);
  Serial.print(',');
  Serial.println(rpyValues[pin][2], 4);
}

void taskPrintAccel_Raw(int pin)
{
  Serial.print(accelValuesRaw[pin][0], 1);
  Serial.print(',');
  Serial.print(accelValuesRaw[pin][1], 1);
  Serial.print(',');
  Serial.println(accelValuesRaw[pin][2], 1);
}

void taskPrintAccel_Mss(int pin)
// Mss Normalized to 1g
{
  Serial.print(g * accelValuesMss[pin][0], 4);
  Serial.print(',');
  Serial.print(g * accelValuesMss[pin][1], 4);
  Serial.print(',');
  Serial.println(g * accelValuesMss[pin][2], 4);
}

void taskPrintGyro(int pin)
{
  Serial.print(gyroValues[pin][0], 4);
  Serial.print(',');
  Serial.print(gyroValues[pin][1], 4);
  Serial.print(',');
  Serial.println(gyroValues[pin][2], 4);
}

void taskPrintMag(int pin)
{
  Serial.print(magValues[pin][0], 4);
  Serial.print(',');
  Serial.print(magValues[pin][1], 4);
  Serial.print(',');
  Serial.println(magValues[pin][2], 4);
}

void taskPrintMarg(int pin)
{
  Serial.print("Raw:");
  Serial.print((int)round(accelValuesMss[pin][0] * 835.41));
  Serial.print(',');
  Serial.print((int)round(accelValuesMss[pin][1] * 835.41));
  Serial.print(',');
  Serial.print((int)round(accelValuesMss[pin][2] * 835.41));
  Serial.print(',');
  Serial.print((int)round(gyroValues[pin][0] * 16));
  Serial.print(',');
  Serial.print((int)round(gyroValues[pin][1] * 16));
  Serial.print(',');
  Serial.print((int)round(gyroValues[pin][2] * 16));
  Serial.print(',');
  Serial.print((int)round(magValues[pin][0] * 10));
  Serial.print(',');
  Serial.print((int)round(magValues[pin][1] * 10));
  Serial.print(',');
  Serial.print((int)round(magValues[pin][2] * 10));
  Serial.println();
}

void plantPrintAll()
{
  // prints angle displacement
  Serial.print(plantPhi(0));
  Serial.print(",");
  Serial.print(plantTheta(0));
  Serial.print(",");
  Serial.print(plantPsi(0));
  Serial.print(",");
  Serial.println(plantLambda(0));
}

void taskPrintDebug()
{
  Serial.print(rpyValues[2][0]);
  Serial.print(",");
  Serial.print(gyroValues[2][1]);
  Serial.print(",");
  Serial.println(adc[2]);
}
//========================== Plant Functions
void taskPrintOmega()
{
  Serial.print(omega[0]); // phi
  Serial.print(",");
  Serial.print(omega[1]); // theta
  Serial.print(",");
  Serial.println(omega[2]); // psi
}

void taskConverge()
{
  testConverge(0, 1000, 0.5, 0.005);
}

void taskCalculateRPY_Rads()
{
  for (int i = 0; i < 6; i++)
  {
    rpyValues[i] = mpu9250[i].calculateEuler();
    //  rpyValues[i] = mpu9250[i].calculateRPYRadians();
    gyroValues[i] = mpu9250[i].getGyro();
    accelValuesMss[i] = mpu9250[i].getAccelMss();
    magValues[i] = mpu9250[i].getMag();
  }
  /*
    for (int i = 0; i < 6; i++)
    {
      if (i == 1)
      {
        offsetValues[i][1] = rpyValues[i][1] + M_PI;
      }
      else
        offsetValues[i] = rpyValues[i];
    }
  */
  phiRPY[0] = sensorDiff(-1, rpyValues[0][0], rpyValues[1][0]);
  phiRPY[1] = sensorDiff(-1, rpyValues[0][1], rpyValues[1][1]);
  phiRPY[2] = sensorDiff(1, rpyValues[0][2], rpyValues[1][2]);
  thetaRPY[0] = sensorDiff(-1, rpyValues[2][0], rpyValues[3][0]);
  thetaRPY[1] = sensorDiff(-1, rpyValues[2][1], rpyValues[3][1]);
  thetaRPY[2] = sensorDiff(-1, rpyValues[2][2], rpyValues[3][2]);
  psiRPY[0] = sensorDiff(1, rpyValues[4][0], rpyValues[5][0]);
  psiRPY[1] = sensorDiff(-1, rpyValues[4][1], rpyValues[5][1]);
  psiRPY[2] = sensorDiff(1, rpyValues[4][2], rpyValues[5][2]);

  for (int i = 0; i < 3; i++)
  {

    // angle phi is equal to gain times sensor 1 angle [i] subtract sensor sensor 2 angle [i]
    dotPhi[i] = sensorDiff(-1, gyroValues[0][i], gyroValues[1][i]);   // same as above
                                                                      // angle phi is equal to gain times sensor 1 angle [i] subtract sensor sensor 2 angle [i]
    dotTheta[i] = sensorDiff(-1, gyroValues[2][i], gyroValues[3][i]); // same as above
                                                                      // angle phi is equal to gain times sensor 1 angle [i] subtract sensor sensor 2 angle [i]
    dotPsi[i] = sensorDiff(-1, gyroValues[4][i], gyroValues[5][i]);   // same as above
  }
}

void taskCalculateEuler()

{
  for (int i = 0; i < 6; i++)
  {
    rpyValues[i] = mpu9250[i].calculateEuler();
    gyroValues[i] = mpu9250[i].getGyro();
    accelValuesMss[i] = mpu9250[i].getAccelMss();
    magValues[i] = mpu9250[i].getMag();
  }
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2

  vectorRPY = bnoSensor.getVector(Adafruit_BNO055::VECTOR_EULER);      // get from bno
  vectorGyro = bnoSensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); // is deg/s NOT rad/s
  vectorAccel = bnoSensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  vectorMag = bnoSensor.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  for (int i = 0; i < 3; i++)
  {
    rpyValues[6][i] = vectorRPY[i];
    gyroValues[6][i] = vectorGyro[i];
    accelValuesMss[6][i] = vectorAccel[i];
    magValues[6][i] = vectorMag[i];
    lambdaRPY[i] = rpyValues[6][i] * DEG_TO_RAD;
  }

  phiRPY[0] = sensorMean(-1, rpyValues[0][0], rpyValues[1][0]);
  phiRPY[1] = sensorMean(1, rpyValues[0][1], rpyValues[1][1]);
  phiRPY[2] = sensorMean(1, rpyValues[0][2], rpyValues[1][2]);
  thetaRPY[0] = sensorMean(-1, rpyValues[2][0], rpyValues[3][0]);
  thetaRPY[1] = sensorMean(1, rpyValues[2][1], rpyValues[3][1]);
  thetaRPY[2] = sensorMean(1, rpyValues[2][2], rpyValues[3][2]);
  psiRPY[0] = sensorMean(-1, rpyValues[4][0], rpyValues[5][0]);
  psiRPY[1] = sensorMean(1, rpyValues[4][1], rpyValues[5][1]);
  psiRPY[2] = sensorMean(1, rpyValues[4][2], rpyValues[5][2]);

  /*
    phiRPY[0] = sensorDiff(-1, rpyValues[0][0], rpyValues[1][0]);
    phiRPY[1] = sensorDiff(-1, rpyValues[0][1], rpyValues[1][1]);
    phiRPY[2] = sensorDiff(1, rpyValues[0][2], rpyValues[1][2]);
    thetaRPY[0] = sensorDiff(-1, rpyValues[2][0], rpyValues[3][0]);
    thetaRPY[1] = sensorDiff(-1, rpyValues[2][1], rpyValues[3][1]);
    thetaRPY[2] = sensorDiff(-1, rpyValues[2][2], rpyValues[3][2]);
    psiRPY[0] = sensorDiff(1, rpyValues[4][0], rpyValues[5][0]);
    psiRPY[1] = sensorDiff(-1, rpyValues[4][1], rpyValues[5][1]);
    psiRPY[2] = sensorDiff(1, rpyValues[4][2], rpyValues[5][2]);
  */
  for (int i = 0; i < 3; i++)
  {
    // angle phi is equal to gain times sensor 1 angle [i] subtract sensor sensor 2 angle [i]
    dotPhi[i] = sensorMean(-1, gyroValues[0][i], gyroValues[1][i]);   // same as above
    dotTheta[i] = sensorMean(-1, gyroValues[2][i], gyroValues[3][i]); // same as above
    dotPsi[i] = sensorMean(-1, gyroValues[4][i], gyroValues[5][i]);   // same as above
    dotLambda[i] = gyroValues[6][i] * -1 * DEG_TO_RAD;
  }
}

void taskUpdatePlant()
{
  plantPhi(0) = phiRPY[0] + offsetAngle[0]; // Alpha Angle is required
  plantPhi(1) = dotPhi[1]; // same as above
  plantPhi(2) = omega[0];

  plantTheta(0) = -1 * thetaRPY[0] + offsetAngle[1];
  plantTheta(1) = dotTheta[1];
  plantTheta(2) = omega[1];

  plantPsi(0) = -1 * psiRPY[0] + offsetAngle[2];
  plantPsi(1) = dotPsi[1];
  plantPsi(2) = omega[2];

  plantLambda(0) = lambdaRPY[0] - M_PI_2 + offsetAngle[3]; // add offset for bno starting angle
  plantLambda(1) = dotLambda[2];
  plantLambda(2) = omega[2];
}

void taskGetOmega()
{
  // Read TI ADS1015
  for (int i = 0; i < 2; i++)
  {
    adc[i] = readADC(i);
  }
  adc[2] = readADC(3); // ADC 2 is dead on the logic board using ADCnumber 3

  // Update Omega Array
  omega[0] = pwmToRads(adc[2]); // motors matched to subsystem
  omega[1] = pwmToRads(adc[0]);
  omega[2] = pwmToRads(adc[1]);
}

void taskUpdateLQRAll()
{
  u[0] = updateLQR(lqrPhi, plantPhi);
  u[1] = updateLQR(lqrTheta, plantTheta);
  u[2] = updateLQR(lqrPsi, plantPsi);
  // u[2] = updateLQR(lqrLambda, plantLambda);
}

void taskUpdateLQRPhi()
{
  u[0] = updateLQR(lqrPhi, plantPhi);
}

void taskUpdateLQRTheta()
{

  u[1] = updateLQR(lqrTheta, plantTheta);
}

void taskUpdateLQRPsi()
{
  u[2] = updateLQR(lqrPsi, plantPsi) ;
}

void taskCurrentControllerAll()
{
  // motorControl(signal, channel)
  motorControl(u[0], 0); // update Phi
  motorControl(u[1], 1); // update Theta
  motorControl(u[2], 2); // update Psi
}

void taskCurrentControllerPhi()
{
  motorControl(u[0], 0); // update Phi
}

void taskCurrentControllerTheta()
{
  motorControl(u[1], 1); // update Theta
}

void taskCurrentControllerPsi()
{
  motorControl(u[2], 2); // update Psi
}

void taskUpdateFullState()
{
  taskCalculateEuler();
  //  taskCalculateRPY_Rads();
  taskUpdatePlant();
  taskGetOmega();
  taskUpdateLQRAll();
  //  taskCurrentController();
}

//=========== Task Helper Functions ===========

void testConverge(int pin, int iteration, float range, float dt)
{
  bool test;
  static int iter = 0;
  if (converger.isFirstIteration())
  {
    iter = 0;
    test = 0;
  }
  test = mpu9250[pin].converge(rpyValues[pin][0], range, dt);
  iter = iter + test;
  if (iter == iteration && sensorReady.isFirstIteration())
  {
    runner.addTask(sensorReady);
    sensorReady.enable();
    converger.disable();
    runner.deleteTask(converger);
  }
  if (iter == iteration && sensorReady.isLastIteration())
  {
    runner.addTask(sensorReady);
    sensorReady.restart();
    sensorReady.enable();
    converger.disable();
    runner.deleteTask(converger);
    Serial.println("Converged b");
  }
}

void readyLED()
{
  if (sensorReady.isFirstIteration())
  {
    LED_state = false;
  }
  if (LED_state)
  {
    LEDOff();
    LED_state = false;
  }
  else
  {
    LEDOn();
    LED_state = true;
  }
  if (sensorReady.isLastIteration())
  {
    LED_state = false;
    LEDOff();
    sensorReady.disable();
    runner.deleteTask(sensorReady);
    blinker.enable();
  }
}

void blinkCB()
{
  if (blinker.isFirstIteration())
  {
    LED_state = false;
  }
  if (LED_state)
  {
    LEDOff();
    LED_state = false;
  }
  else
  {
    LEDOn();
    LED_state = true;
  }
}

//========================== RPY in Rads Functions
void cs26Print_Rads()
{
  taskPrint_Rads(cs26);
}

void cs14Print_Rads()
{
  taskPrint_Rads(cs14);
}

void cs21Print_Rads()
{
  taskPrint_Rads(cs21);
}

void cs27Print_Rads()
{
  taskPrint_Rads(cs27);
}

void cs32Print_Rads()
{
  taskPrint_Rads(cs32);
}

void cs25Print_Rads()
{
  taskPrint_Rads(cs25);
}
void bnoPrint_Rads()
{
  taskPrint_Rads(bno);
}

//========================== Accel Raw Functions
void cs26PrintAccel_Raw()
{
  taskPrintAccel_Raw(cs26);
}

void cs14PrintAccel_Raw()
{
  taskPrintAccel_Raw(cs14);
}

void cs21PrintAccel_Raw()
{
  taskPrintAccel_Raw(cs21);
}

void cs27PrintAccel_Raw()
{
  taskPrintAccel_Raw(cs27);
}

void cs32PrintAccel_Raw()
{
  taskPrintAccel_Raw(cs32);
}

void cs25PrintAccel_Raw()
{
  taskPrintAccel_Raw(cs25);
}

void bnoPrintAccel_Raw()
{
  taskPrintAccel_Raw(bno);
}
//========================== Accel Mss Functions
void cs26PrintAccel_Mss()
{
  taskPrintAccel_Mss(cs26);
}

void cs14PrintAccel_Mss()
{
  taskPrintAccel_Mss(cs14);
}

void cs21PrintAccel_Mss()
{
  taskPrintAccel_Mss(cs21);
}

void cs27PrintAccel_Mss()
{
  taskPrintAccel_Mss(cs27);
}

void cs32PrintAccel_Mss()
{
  taskPrintAccel_Mss(cs32);
}

void cs25PrintAccel_Mss()
{
  taskPrintAccel_Mss(cs25);
}

void bnoPrintAccel_Mss()
{
  taskPrintAccel_Mss(bno);
}

//========================== MARG For Calibration
void cs26PrintMarg()
{
  taskPrintMarg(cs26);
}

void cs14PrintMarg()
{
  taskPrintMarg(cs14);
}

void cs21PrintMarg()
{
  taskPrintMarg(cs21);
}

void cs27PrintMarg()
{
  taskPrintMarg(cs27);
}

void cs32PrintMarg()
{
  taskPrintMarg(cs32);
}

void cs25PrintMarg()
{
  taskPrintMarg(cs25);
}

//========================== Gyro Functions
void cs26PrintGyro()
{
  taskPrintGyro(cs26);
}

void cs14PrintGyro()
{
  taskPrintGyro(cs14);
}

void cs21PrintGyro()
{
  taskPrintGyro(cs21);
}

void cs27PrintGyro()
{
  taskPrintGyro(cs27);
}

void cs32PrintGyro()
{
  taskPrintGyro(cs32);
}

void cs25PrintGyro()
{
  taskPrintGyro(cs25);
}

void bnoPrintGyro()
{
  taskPrintGyro(bno);
}
//========================== Plant Functions
void plantPrintPhi()
{
  taskPrintPlant(plantPhi);
}

void plantPrintTheta()
{
  taskPrintPlant(plantTheta);
}

void plantPrintPsi()
{
  taskPrintPlant(plantPsi);
}

void plantPrintLambda()
{
  taskPrintPlant(plantLambda);
}