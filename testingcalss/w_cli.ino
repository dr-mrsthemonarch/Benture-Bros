Command cmdMotorPrint;
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
Command cmdLQROn;
Command cmdLQR;      // Turn on motors
Command cmdLQRPrint; // Print
Command modLQR;      // Mod Gains
Command cmdDebug;
Command cmdprintAll;

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

void lqrPrintCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  bool lqrOn = argOn.isSet();
  bool lqrOff = argOff.isSet();

  if (lqrOn)
  {
    runner.addTask(printU);
    printU.enable();
  }
  else
  {
    printU.disable();
    runner.deleteTask(printU);
  }
}

void printAllCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  bool printAllOn = argOn.isSet();
  bool printAllOff = argOff.isSet();

  if (printAllOn)
  {
    runner.addTask(printAll);
    printAll.enable();
  }
  else if (printAllOff)
  {
    printAll.disable();
    runner.deleteTask(printAll);
  }
}

void lqrCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing
  Argument argC = cmd.getArgument("c");              // turn on/off controller with on argument

  Argument argSub = cmd.getArgument("s");
  String subp = argSub.getValue();
  int pi = subp.toInt();

  bool c = argC.isSet();
  bool lqrOn = argOn.isSet();
  bool lqrOff = argOff.isSet();

  if (lqrOn && c)
  {
    switch (pi)
    {
    case 0:
      enableMotor();
      runner.addTask(currentControllerAll);
      currentControllerAll.enable();
      Serial.println("Controller All On");
      break;
    case 1:
      enableMotor();
      runner.addTask(currentControllerPhi);
      currentControllerPhi.enable();
      Serial.println("Controller Phi On");
      break;
    case 2:
      enableMotor();
      runner.addTask(currentControllerTheta);
      currentControllerTheta.enable();
      Serial.print("Controller Theta On");
      break;
    case 3:
      enableMotor();
      runner.addTask(currentControllerPsi);
      currentControllerPsi.enable();
      Serial.println("Controller Psi On");
      break;
    }
  }
  else if (lqrOff && c)
  {
    switch (pi)
    {
    case 0:
      disableMotor();
      currentControllerAll.disable();
      runner.deleteTask(currentControllerAll);
      Serial.println("Controller All Off");
      break;
    case 1:
      disableMotor();
      currentControllerPhi.disable();
      runner.deleteTask(currentControllerPhi);
      Serial.println("Controller Phi Off");
      break;
    case 2:
      disableMotor();
      currentControllerTheta.disable();
      runner.deleteTask(currentControllerTheta);
      Serial.print("Controller Theta Off");
      break;
    case 3:
      disableMotor();
      currentControllerPsi.disable();
      runner.deleteTask(currentControllerPsi);
      Serial.println("Controller Psi Off");
      break;
    }
  }
}

void lqrStartCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                               // get arguments
  Argument argOn = cmd.getArgument("e/nable,on");    // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off"); // turn on printing

  Argument argSub = cmd.getArgument("s");
  String subp = argSub.getValue();
  int pi = subp.toInt();

  bool lqrOn = argOn.isSet();
  bool lqrOff = argOff.isSet();
  if (lqrOn)
  {
    switch (pi)
    {
    case 0:
      runner.addTask(controlLQRAll);
      controlLQRAll.enable();
      Serial.println("LQR All On");
      break;
    case 1:
      runner.addTask(controlLQRPhi);
      controlLQRPhi.enable();
      Serial.println("LQR Phi On");
      break;
    case 2:
      runner.addTask(controlLQRTheta);
      controlLQRTheta.enable();
      Serial.print("LQR Theta On");
      break;
    case 3:
      runner.addTask(controlLQRPsi);
      controlLQRPsi.enable();
      Serial.println("LQR Psi On");
      break;
    }
  }
  else if (lqrOff)
  {
    switch (pi)
    {
    case 0:
      controlLQRAll.disable();
      runner.deleteTask(controlLQRAll);
      Serial.println("LQR All Off");
      break;
    case 1:
      controlLQRPhi.disable();
      runner.deleteTask(controlLQRPhi);
      Serial.println("LQR Phi Off");
      break;
    case 2:
      controlLQRTheta.disable();
      runner.deleteTask(controlLQRTheta);
      Serial.print("LQR Theta Off");
      break;
    case 3:
      controlLQRPsi.disable();
      runner.deleteTask(controlLQRPsi);
      Serial.println("LQR Psi Off");
      break;
    }
  }
}

void lqrSetCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr); // get arguments

  Argument argK1 = cmd.getArgument("k1");
  String k1 = argK1.getValue();
  float gain1 = k1.toFloat();

  Argument argK2 = cmd.getArgument("k2");
  String k2 = argK2.getValue();
  float gain2 = k2.toFloat();

  Argument argK3 = cmd.getArgument("k3");
  String k3 = argK3.getValue();
  float gain3 = k3.toFloat();

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
    Serial.print(lqrTheta(0));
    Serial.print(",");
    Serial.print(lqrTheta(1));
    Serial.print(",");
    Serial.println(lqrTheta(2));
    break;
  case 2:
    lqrLambda(0) = gain1;
    lqrLambda(1) = gain2;
    lqrLambda(2) = gain3;
    lqrPsi(0) = gain1;
    lqrPsi(1) = gain2;
    lqrPsi(2) = gain3;
    Serial.print(lqrLambda(0));
    Serial.print(",");
    Serial.print(lqrLambda(1));
    Serial.print(",");
    Serial.println(lqrLambda(2));
    break;
  }
}

void motorPrintCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                            // get arguments
  Argument argOn = cmd.getArgument("e/nable,on"); // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off");
  bool motorOn = argOn.isSet();
  bool motorOff = argOff.isSet();

  if (motorOn)
  {
    runner.addTask(printOmega);
    printOmega.enable();
  }
  else if (motorOff)
  {
    printOmega.disable();
    runner.deleteTask(printOmega);
  }
}

void motorStateCB(cmd *cmdPtr)
{
  Command cmd(cmdPtr);                            // get arguments
  Argument argOn = cmd.getArgument("e/nable,on"); // turn off printing
  Argument argOff = cmd.getArgument("d/isable,off");
  Argument argPrint = cmd.getArgument("p/rint");

  bool motorOn = argOn.isSet();
  bool motorOff = argOff.isSet();
  bool motorPrint = argPrint.isSet();

  if (motorOn)
  {
    enableMotor();
  }
  else if (motorOff)
  {
    disableMotor();
  }
  else if (motorPrint && motorOn)
  {
    runner.addTask(printOmega);
    printOmega.enable();
  }
  else if (motorPrint && motorOff)
  {
    printOmega.disable();
    runner.deleteTask(printOmega);
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
      motorControl(driveCurrent, 0, -1);
      break;

    case 2:
      enableMotor();
      motorControl(driveCurrent, 1, -1);
      break;

    case 3:
      enableMotor();
      motorControl(driveCurrent, 2, -1);
      break;

    case 0:
      enableMotor();
      motorControl(driveCurrent, 0, -1);
      motorControl(driveCurrent, 1, -1);
      motorControl(driveCurrent, 2, -1);
      break;
    }
}

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
    case 77:
      runner.addTask(bno_RPY);
      bno_RPY.enable();
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
    case 77:
      bno_RPY.disable();
      runner.deleteTask(bno_RPY);
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
    case 77:
      runner.addTask(bno_Accel_Raw);
      bno_Accel_Raw.enable();
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
    case 77:
      bno_Accel_Raw.disable();
      runner.deleteTask(bno_Accel_Raw);
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
    case 77:
      runner.addTask(bno_Accel_Mss);
      bno_Accel_Mss.enable();
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
    case 77:
      bno_Accel_Mss.disable();
      runner.deleteTask(bno_Accel_Mss);
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
    case 77:
      runner.addTask(bno_Gyro);
      bno_Gyro.enable();
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
    case 77:
      bno_Gyro.disable();
      runner.deleteTask(bno_Gyro);
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

  Argument argCS = cmd.getArgument("p");
  String angle = argCS.getValue();
  int angleSelect = angle.toInt();

  bool sensorOn = argOn.isSet();
  bool sensorOff = argOff.isSet();

  if (sensorOn)
  {
    switch (angleSelect)
    {
    case 0:
      runner.addTask(printPhiThetaPsiLambda);
      printPhiThetaPsiLambda.enable();
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
    case 4:
      runner.addTask(printLambda);
      printLambda.enable();
      break;
    }
  }
  else if (sensorOff)
  {
    switch (angleSelect)
    {
    case 0:
      printPhiThetaPsiLambda.disable();
      runner.deleteTask(printPhiThetaPsiLambda);
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
    case 4:
      printLambda.disable();
      runner.deleteTask(printLambda);
      break;
    }
  }
}
