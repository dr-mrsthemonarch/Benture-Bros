
Command cmdMotorState;
Command cmdMotor;
Command cmdConverge;
Command cmdHelp;
Command cmdGetAccelRaw;
Command cmdGetAccelMss;
Command cmdGetGyro;
Command cmdGetRPY;
Command cmdGetMarg;
Command cmdGetPlant;
Command cmdLQR;
Command modLQR;
Command cmdDebug;

// https://github.com/SpacehuhnTech/SimpleCLI#examples
// Commands must be initiated as global void with cmd* Prt, Bare Minimum example:
// void example(cmd * Pointer){
// Command cmd(Pointer);
// doWhateverHere()
//}

void errorCallback(cmd_error *errorPtr)
{
  CommandError e(errorPtr);

  Serial.println("ERROR: " + e.toString());

  if (e.hasCommand())
  {
    Serial.println("Did you mean? " + e.getCommand().toString());
  }
  else
  {
    Serial.println(cli.toString());
  }
}
void helpCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Serial.println("Help:");
  Serial.println(cli.toString());
}

void debugCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  bool printOn = argOn.isSet();
  bool printOff = argOff.isSet();

  if (printOn)
  {
    runner.addTask(printDebug);
    printDebug.enable();
  }
  else if (printOff)
  {
    printDebug.disable();
    runner.deleteTask(printDebug);
  }
}

void convergeCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr); // does nothing, required for creating command
  blinker.disable();
  LED_state = false;
  LEDOff();
  runner.addTask(converger);
  converger.enable();
  Serial.println("command accepted");
}

void lqrCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing
  Argument argC = cmd.getArgument("c");              // turn on/off controller with on argument

  Argument argPrint = cmd.getArgument("p/rint");

  bool c = argC.isSet();
  bool printer = argPrint.isSet();
  bool lqrOn = argOn.isSet();
  bool lqrOff = argOff.isSet();

  if (lqrOn && printer)
  {
    runner.addTask(printU);
    printU.enable();
  }
  else if (lqrOff && printer)
  {
    printU.disable();
    runner.deleteTask(printU);
  }
  else if (lqrOn && c)
  {
    motorDriver.enable();
  }
  else if (lqrOff && c)
  {
    motorDriver.disable();
  }
}

void lqrSetCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr); // get arguments

  Argument argK1 = cmd.getArgument("k1");
  String k1 = argK1.getValue();
  float gain1 = k1.toInt();

  Argument argK2 = cmd.getArgument("k2");
  String k2 = argK2.getValue();
  float gain2 = k2.toInt();

  Argument argK3 = cmd.getArgument("k3");
  String k3 = argK3.getValue();
  float gain3 = k3.toInt();

  Argument argSub = cmd.getArgument("p");
  String subp = argSub.getValue();
  int pi = subp.toInt();

  switch (pi)
  {
  case 0:
    lqrPhi(0) = gain1;
    lqrPhi(1) = gain2;
    lqrPhi(2) = gain3;
    Serial.print(lqrPhi(0));
    Serial.print(",");
    Serial.print(lqrPhi(1));
    Serial.print(",");
    Serial.println(lqrPhi(2));
    break;
  case 1:
    lqrTheta(0) = gain1;
    lqrTheta(1) = gain2;
    lqrTheta(2) = gain3;
    Serial.print(lqrTheta(1));
    Serial.print(",");
    Serial.print(lqrTheta(2));
    Serial.print(",");
    Serial.println(lqrTheta(2));
    break;
  case 2:
    lqrPsi(0) = gain1;
    lqrPsi(1) = gain2;
    lqrPsi(2) = gain3;
    Serial.print(lqrPsi(0));
    Serial.print(",");
    Serial.print(lqrPsi(1));
    Serial.print(",");
    Serial.println(lqrPsi(2));
    break;
  }
}
/*
  use cases
  lqr -on -print        turn on all controller values printing
  lqr -off -print       turns off all printing
  lqr -on -c            turns on all controllers
  lqr -off -c           turns off all controllers

  lqr -s/et -p 0,1,2 -k1/2/3 150.2

*/

void getRPYCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  Argument argCS = cmd.getArgument("c");
  String pin = argCS.getValue();
  int csPin = pin.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (csPin)
    {
    case 26:
      runner.addTask(cs26_RPY);
      cs26_RPY.enable();
      break;
    case 14:
      runner.addTask(cs14_RPY);
      cs14_RPY.enable();
      break;
    case 21:
      runner.addTask(cs21_RPY);
      cs21_RPY.enable();
      break;
    case 27:
      runner.addTask(cs27_RPY);
      cs27_RPY.enable();
      break;
    case 32:
      runner.addTask(cs32_RPY);
      cs32_RPY.enable();
      break;
    case 25:
      runner.addTask(cs25_RPY);
      cs25_RPY.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (csPin)
    {
    case 26:
      cs26_RPY.disable();
      runner.deleteTask(cs26_RPY);
      break;
    case 14:
      cs14_RPY.disable();
      runner.deleteTask(cs14_RPY);
      break;
    case 21:
      cs21_RPY.disable();
      runner.deleteTask(cs21_RPY);
      break;
    case 27:
      cs27_RPY.disable();
      runner.deleteTask(cs27_RPY);
      break;
    case 32:
      cs32_RPY.disable();
      runner.deleteTask(cs32_RPY);
      break;
    case 25:
      cs25_RPY.disable();
      runner.deleteTask(cs25_RPY);
      break;
    }
  }
}
void getAccelRawCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing
  Argument argCS = cmd.getArgument("c");
  String pin = argCS.getValue();
  int csPin = pin.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (csPin)
    {
    case 26:
      runner.addTask(cs26_Accel_Raw);
      cs26_Accel_Raw.enable();
      break;
    case 14:
      runner.addTask(cs14_Accel_Raw);
      cs14_Accel_Raw.enable();
      break;
    case 21:
      runner.addTask(cs21_Accel_Raw);
      cs21_Accel_Raw.enable();
      break;
    case 27:
      runner.addTask(cs27_Accel_Raw);
      cs27_Accel_Raw.enable();
      break;
    case 32:
      runner.addTask(cs32_Accel_Raw);
      cs32_Accel_Raw.enable();
      break;
    case 25:
      runner.addTask(cs25_Accel_Raw);
      cs25_Accel_Raw.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (csPin)
    {
    case 26:
      cs26_Accel_Raw.disable();
      runner.deleteTask(cs26_Accel_Raw);
      break;
    case 14:
      cs14_Accel_Raw.disable();
      runner.deleteTask(cs14_Accel_Raw);
      break;
    case 21:
      cs21_Accel_Raw.disable();
      runner.deleteTask(cs21_Accel_Raw);
      break;
    case 27:
      cs27_Accel_Raw.disable();
      runner.deleteTask(cs27_Accel_Raw);
      break;
    case 32:
      cs32_Accel_Raw.disable();
      runner.deleteTask(cs32_Accel_Raw);
      break;
    case 25:
      cs25_Accel_Raw.disable();
      runner.deleteTask(cs25_Accel_Raw);
      break;
    }
  }
}
void getAccelMssCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  Argument argCS = cmd.getArgument("c");
  String pin = argCS.getValue();
  int csPin = pin.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (csPin)
    {
    case 26:
      runner.addTask(cs26_Accel_Mss);
      cs26_Accel_Mss.enable();
      break;
    case 14:
      runner.addTask(cs14_Accel_Mss);
      cs14_Accel_Mss.enable();
      break;
    case 21:
      runner.addTask(cs21_Accel_Mss);
      cs21_Accel_Mss.enable();
      break;
    case 27:
      runner.addTask(cs27_Accel_Mss);
      cs27_Accel_Mss.enable();
      break;
    case 32:
      runner.addTask(cs32_Accel_Mss);
      cs32_Accel_Mss.enable();
      break;
    case 25:
      runner.addTask(cs25_Accel_Mss);
      cs25_Accel_Mss.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (csPin)
    {
    case 26:
      cs26_Accel_Mss.disable();
      runner.deleteTask(cs26_Accel_Mss);
      break;
    case 14:
      cs14_Accel_Mss.disable();
      runner.deleteTask(cs14_Accel_Mss);
      break;
    case 21:
      cs21_Accel_Mss.disable();
      runner.deleteTask(cs21_Accel_Mss);
      break;
    case 27:
      cs27_Accel_Mss.disable();
      runner.deleteTask(cs27_Accel_Mss);
      break;
    case 32:
      cs32_Accel_Mss.disable();
      runner.deleteTask(cs32_Accel_Mss);
      break;
    case 25:
      cs25_Accel_Mss.disable();
      runner.deleteTask(cs25_Accel_Mss);
      break;
    }
  }
}

void getMargCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  Argument argCS = cmd.getArgument("c");
  String pin = argCS.getValue();
  int csPin = pin.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (csPin)
    {
    case 26:
      runner.addTask(cs26_Marg);
      cs26_Marg.enable();
      break;
    case 14:
      runner.addTask(cs14_Marg);
      cs14_Marg.enable();
      break;
    case 21:
      runner.addTask(cs21_Marg);
      cs21_Marg.enable();
      break;
    case 27:
      runner.addTask(cs27_Marg);
      cs27_Marg.enable();
      break;
    case 32:
      runner.addTask(cs32_Marg);
      cs32_Marg.enable();
      break;
    case 25:
      runner.addTask(cs25_Marg);
      cs25_Marg.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (csPin)
    {
    case 26:
      cs26_Marg.disable();
      runner.deleteTask(cs26_Marg);
      break;
    case 14:
      cs14_Marg.disable();
      runner.deleteTask(cs14_Marg);
      break;
    case 21:
      cs21_Marg.disable();
      runner.deleteTask(cs21_Marg);
      break;
    case 27:
      cs27_Marg.disable();
      runner.deleteTask(cs27_Marg);
      break;
    case 32:
      cs32_Marg.disable();
      runner.deleteTask(cs32_Marg);
      break;
    case 25:
      cs25_Marg.disable();
      runner.deleteTask(cs25_Marg);
      break;
    }
  }
}
void getGyroCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  Argument argCS = cmd.getArgument("c");
  String pin = argCS.getValue();
  int csPin = pin.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (csPin)
    {
    case 26:
      runner.addTask(cs26_Gyro);
      cs26_Gyro.enable();
      break;
    case 14:
      runner.addTask(cs14_Gyro);
      cs14_Gyro.enable();
      break;
    case 21:
      runner.addTask(cs21_Gyro);
      cs21_Gyro.enable();
      break;
    case 27:
      runner.addTask(cs27_Gyro);
      cs27_Gyro.enable();
      break;
    case 32:
      runner.addTask(cs32_Gyro);
      cs32_Gyro.enable();
      break;
    case 25:
      runner.addTask(cs25_Gyro);
      cs25_Gyro.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (csPin)
    {
    case 26:
      cs26_Gyro.disable();
      runner.deleteTask(cs26_Gyro);
      break;
    case 14:
      cs14_Gyro.disable();
      runner.deleteTask(cs14_Gyro);
      break;
    case 21:
      cs21_Gyro.disable();
      runner.deleteTask(cs21_Gyro);
      break;
    case 27:
      cs27_Gyro.disable();
      runner.deleteTask(cs27_Gyro);
      break;
    case 32:
      cs32_Gyro.disable();
      runner.deleteTask(cs32_Gyro);
      break;
    case 25:
      cs25_Gyro.disable();
      runner.deleteTask(cs25_Gyro);
      break;
    }
  }
}
void getPlantCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing
  Argument argAll = cmd.getArgument("all");

  Argument argCS = cmd.getArgument("s");
  String angle = argCS.getValue();
  int angleSelect = angle.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (angleSelect)
    {
    case 0:
      runner.addTask(printPhiThetaPsi);
      printPhiThetaPsi.enable();
      break;
    case 1:
      runner.addTask(printPhi);
      printPhi.enable();
      break;
    case 2:
      runner.addTask(printTheta);
      printTheta.enable();
      break;
    case 3:
      runner.addTask(printPsi);
      printPsi.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (angleSelect)
    {
    case 0:
      printPhiThetaPsi.disable();
      runner.deleteTask(printPhiThetaPsi);
      break;
    case 1:
      printPhi.disable();
      runner.deleteTask(printPhi);
      break;
    case 2:
      printTheta.disable();
      runner.deleteTask(printTheta);
      break;
    case 3:
      printPsi.disable();
      runner.deleteTask(printPsi);
      break;
    }
  }
}

void motorStateCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing
  bool motorOn = argOn.isSet();
  bool motorOff = argOn.isSet();

  if (motorOn)
  {
    enableMotor();
  }
  else if (motorOff)
  {
    disableMotor();
  }
}

void motorControlCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr); // get arguments

  Argument argAmps = cmd.getArgument("a/mps");
  String amps = argAmps.getValue();
  float driveCurrent = amps.toFloat();

  Argument argSub = cmd.getArgument("p");
  String motor = argSub.getValue();
  int motorSelect = motor.toInt();

  if (motorSelect >= 0)
    switch (motorSelect)
    {
    case 1:
      enableMotor();
      motorControl(driveCurrent, 0);
      break;

    case 2:
      enableMotor();
      motorControl(driveCurrent, 1);
      break;

    case 3:
      enableMotor();
      motorControl(driveCurrent, 2);
      break;

    case 0:
      enableMotor();
      motorControl(driveCurrent, 0);
      motorControl(driveCurrent, 1);
      motorControl(driveCurrent, 2);
      break;
    }
}
