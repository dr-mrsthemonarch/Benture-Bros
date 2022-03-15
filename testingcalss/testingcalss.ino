#include "header.hpp"

//Prototypes for tasks
//========================== Accel Raw
void cs26PrintAccel_Raw();
void cs14PrintAccel_Raw();
void cs21PrintAccel_Raw();
void cs27PrintAccel_Raw();
void cs32PrintAccel_Raw();
void cs25PrintAccel_Raw();
//========================== Accel Mss
void cs26PrintAccel_Mss();
void cs14PrintAccel_Mss();
void cs21PrintAccel_Mss();
void cs27PrintAccel_Mss();
void cs32PrintAccel_Mss();
void cs25PrintAccel_Mss();
//========================== RPY Rads
void cs26PrintRPY_Rads();
void cs14PrintRPY_Rads();
void cs21PrintRPY_Rads();
void cs27PrintRPY_Rads();
void cs32PrintRPY_Rads();
void cs25PrintRPY_Rads();
//========================== Marg
void cs26PrintMarg();
void cs14PrintMarg();
void cs21PrintMarg();
void cs27PrintMarg();
void cs32PrintMarg();
void cs25PrintMarg();

void taskCalculateRPY();
void blinkCB();
void taskConverge();
void readyLED();
void taskCalculateRPY_Rads();
void taskReadSerial();
//void taskADC();



//Tasks to always run
Task blinker(500, TASK_FOREVER, &blinkCB);
//Task probeADC(10, TASK_FOREVER, &taskADC);
Task angleCalculateRPY(3, TASK_FOREVER, &taskCalculateRPY);
Task angleCalculateRPY_Rads(3, TASK_FOREVER, &taskCalculateRPY_Rads);

//Dynamic Tasks
Task sensorReady(50, 50, &readyLED);
Task converger(11, TASK_FOREVER, &taskConverge);
Task serialRead(7, TASK_FOREVER, &taskReadSerial);

Task cs26_RPY(5, TASK_FOREVER, &cs26PrintRPY_Rads);
Task cs14_RPY(5, TASK_FOREVER, &cs14PrintRPY_Rads);
Task cs21_RPY(5, TASK_FOREVER, &cs21PrintRPY_Rads);
Task cs27_RPY(5, TASK_FOREVER, &cs27PrintRPY_Rads);
Task cs32_RPY(5, TASK_FOREVER, &cs32PrintRPY_Rads);
Task cs25_RPY(5, TASK_FOREVER, &cs25PrintRPY_Rads);

Task cs26_Accel_Raw(10, TASK_FOREVER, &cs26PrintAccel_Raw);
Task cs14_Accel_Raw(10, TASK_FOREVER, &cs14PrintAccel_Raw);
Task cs21_Accel_Raw(10, TASK_FOREVER, &cs21PrintAccel_Raw);
Task cs27_Accel_Raw(10, TASK_FOREVER, &cs27PrintAccel_Raw);
Task cs32_Accel_Raw(10, TASK_FOREVER, &cs32PrintAccel_Raw);
Task cs25_Accel_Raw(10, TASK_FOREVER, &cs25PrintAccel_Raw);

Task cs26_Accel_Mss(10, TASK_FOREVER, &cs26PrintAccel_Mss);
Task cs14_Accel_Mss(10, TASK_FOREVER, &cs14PrintAccel_Mss);
Task cs21_Accel_Mss(10, TASK_FOREVER, &cs21PrintAccel_Mss);
Task cs27_Accel_Mss(10, TASK_FOREVER, &cs27PrintAccel_Mss);
Task cs32_Accel_Mss(10, TASK_FOREVER, &cs32PrintAccel_Mss);
Task cs25_Accel_Mss(10, TASK_FOREVER, &cs25PrintAccel_Mss);

Task cs26_Marg(50, TASK_FOREVER, &cs26PrintMarg);
Task cs14_Marg(50, TASK_FOREVER, &cs14PrintMarg);
Task cs21_Marg(50, TASK_FOREVER, &cs21PrintMarg);
Task cs27_Marg(50, TASK_FOREVER, &cs27PrintMarg);
Task cs32_Marg(50, TASK_FOREVER, &cs32PrintMarg);
Task cs25_Marg(50, TASK_FOREVER, &cs25PrintMarg);


Scheduler runner;


//=========== Task Functions ===========
void taskReadSerial() {
  if (Serial.available() > 0) {
    stringRead = Serial.readStringUntil('\n');
    schedularOnOff(stringRead.toInt());
    cli.parse(stringRead);
  }
  parser();
}

void taskPrintRPY(int pin) {
  Serial.print(rpyValues[pin][0]);
  Serial.print(',');
  Serial.print(rpyValues[pin][1]);
  Serial.print(',');
  Serial.println(rpyValues[pin][2]);
}
void taskPrintRPY_Rads(int pin) {
  Serial.print(rpyValues[pin][0], 4);
  Serial.print(',');
  Serial.print(rpyValues[pin][1], 4);
  Serial.print(',');
  Serial.println(rpyValues[pin][2], 4);
}
void taskPrintAccel_Raw(int pin) {
  Serial.print(accelValuesRaw[pin][0], 1);
  Serial.print(',');
  Serial.print(accelValuesRaw[pin][1], 1);
  Serial.print(',');
  Serial.println(accelValuesRaw[pin][2], 1);
}
void taskPrintAccel_Mss(int pin) {
  Serial.print(accelValuesMss[pin][0], 4);
  Serial.print(',');
  Serial.print(accelValuesMss[pin][1], 4);
  Serial.print(',');
  Serial.println(accelValuesMss[pin][2], 4);
}
void taskPrintGyro(int pin) {
  Serial.print(gyroValues[pin][0], 4);
  Serial.print(',');
  Serial.print(gyroValues[pin][1], 4);
  Serial.print(',');
  Serial.println(gyroValues[pin][2], 4);
}
void taskPrintMag(int pin) {
  Serial.print(magValues[pin][0], 4);
  Serial.print(',');
  Serial.print(magValues[pin][1], 4);
  Serial.print(',');
  Serial.println(magValues[pin][2], 4);
}
void taskPrintMarg(int pin) {
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
  Serial.print((int)round(magValues[pin][2] * 10 ));
  Serial.println();
}
void taskADC() {
  // Read TI ADS1015
  for (int i = 0; i < 2; i++) {
    adc[i] = readADC(i);
  }
  adc[2] = readADC(3); // ADC 2 is dead on the logic board using ADCnumber 3
}
void taskConverge() {
  testConverge(0, 1000, 0.5, 0.005);
}
void taskCalculateRPY() {
  for (int i = 0 ; i < 2; i++) {
    rpyValues[i] = testing[i].calculateRPY();
    gyroValues[i] = testing[i].getGyro();
    accelValuesRaw[i] = testing[i].getAccelRaw();
    accelValuesMss[i] = testing[i].getAccelMss();
  }
}
void taskCalculateRPY_Rads() {
  for (int i = 0 ; i < 2; i++) {
    rpyValues[i] = testing[i].calculateRPYRadians();
    phiRPY[i] = sensorOffset(1, rpyValues[0][i], rpyValues[1][i]); //angle phi is equal to gain times sensor 1 angle [i] subtract sensor sensor 2 angle [i]
    gyroValues[i] = testing[i].getGyro();
    accelValuesRaw[i] = testing[i].getAccelRaw();
    accelValuesMss[i] = testing[i].getAccelMss();
    magValues[i] = testing[i].getMag();
  }
}

void taskUpdateLQR(){
  updateLQR(lqrPhi,plantPhi);
  updateLQR(lqrTheta,plantTheta);
  updateLQR(lqrPsi,plantPsi);
  }
void taskUpdateMotor(){
  motorControl(0,u[0]); //update Phi
  motorControl(1,u[1]); //update Theta
  motorControl(2,u[2]); //update Psi
  }

//=========== Task Helper Functions ===========

void testConverge(int pin, int iteration, float range, float dt ) {
  bool test;
  static int iter = 0;
  if (converger.isFirstIteration()) {
    iter = 0;
    test = 0;
  }
  test = testing[pin].converge(rpyValues[pin][0], range, dt);
  iter = iter + test;
  if (iter == iteration && sensorReady.isFirstIteration()) {
    runner.addTask(sensorReady);
    sensorReady.enable();
    converger.disable();
    runner.deleteTask(converger);
  }
  if (iter == iteration && sensorReady.isLastIteration()) {
    runner.addTask(sensorReady);
    sensorReady.restart();
    sensorReady.enable();
    converger.disable();
    runner.deleteTask(converger);
    Serial.println("Converged b");
  }
}

void readyLED() {
  if (sensorReady.isFirstIteration() ) {
    LED_state = false;
  }
  if ( LED_state ) {
    LEDOff();
    LED_state = false;
  }

  else {
    LEDOn();
    LED_state = true;
  }
  if (sensorReady.isLastIteration() ) {
    LED_state = false;
    LEDOff();
    sensorReady.disable();
    runner.deleteTask(sensorReady);
    blinker.enable();
  }
}

void blinkCB() {
  if (blinker.isFirstIteration() ) {
    LED_state = false;
  }

  if ( LED_state ) {
    LEDOff();
    LED_state = false;
  }
  else {
    LEDOn();
    LED_state = true;
  }
}

void taskLQR() {


}

//========================== RPY in Rads
void cs26PrintRPY_Rads() {
  taskPrintRPY_Rads(cs26);
}

void cs14PrintRPY_Rads() {
  taskPrintRPY_Rads(cs14);
}

void cs21PrintRPY_Rads() {
  taskPrintRPY_Rads(cs21);
}

void cs27PrintRPY_Rads() {
  taskPrintRPY_Rads(cs27);
}

void cs32PrintRPY_Rads() {
  taskPrintRPY_Rads(cs32);
}

void cs25PrintRPY_Rads() {
  taskPrintRPY_Rads(cs25);
}
//========================== Accel Raw
void cs26PrintAccel_Raw() {
  taskPrintAccel_Raw(cs26);
}

void cs14PrintAccel_Raw() {
  taskPrintAccel_Raw(cs14);
}

void cs21PrintAccel_Raw() {
  taskPrintAccel_Raw(cs21);
}

void cs27PrintAccel_Raw() {
  taskPrintAccel_Raw(cs27);
}

void cs32PrintAccel_Raw() {
  taskPrintAccel_Raw(cs32);
}

void cs25PrintAccel_Raw() {
  taskPrintAccel_Raw(cs25);
}

//========================== Accel Mss
void cs26PrintAccel_Mss() {
  taskPrintAccel_Mss(cs26);
}

void cs14PrintAccel_Mss() {
  taskPrintAccel_Mss(cs14);
}

void cs21PrintAccel_Mss() {
  taskPrintAccel_Mss(cs21);
}

void cs27PrintAccel_Mss() {
  taskPrintAccel_Mss(cs27);
}

void cs32PrintAccel_Mss() {
  taskPrintAccel_Mss(cs32);
}

void cs25PrintAccel_Mss() {
  taskPrintAccel_Mss(cs25);
}
//========================== MARG For Calibration
void cs26PrintMarg() {
  taskPrintMarg(cs26);
}

void cs14PrintMarg() {
  taskPrintMarg(cs14);
}

void cs21PrintMarg() {
  taskPrintMarg(cs21);
}

void cs27PrintMarg() {
  taskPrintMarg(cs27);
}

void cs32PrintMarg() {
  taskPrintMarg(cs32);
}

void cs25PrintMarg() {
  taskPrintMarg(cs25);
}

//========================== LQR Functions
void LQRPhi() {


}
