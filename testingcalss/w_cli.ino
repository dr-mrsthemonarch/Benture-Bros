

Command cmdMotor;
Command cmdConverge;
Command cmdPing;
Command cmdPong;
Command cmdHelp;
Command cmdGetAccelRaw;
Command cmdGetAccelMss;
Command cmdGetRPY;
Command cmdGetMarg;
// https://github.com/SpacehuhnTech/SimpleCLI#examples
// Commands must be initiated as global void with cmd* Prt, Bare Minimum example:
// void example(cmd * Pointer){
// Command cmd(Pointer);
// doWhateverHere()
//}


void errorCallback(cmd_error* errorPtr) {
  CommandError e(errorPtr);

  Serial.println("ERROR: " + e.toString());

  if (e.hasCommand()) {
    Serial.println("Did you mean? " + e.getCommand().toString());
  } else {
    Serial.println(cli.toString());
  }
}
void helpCB(cmd* ptr) {
  Command cmd(ptr);
  Serial.println("Help:");
  Serial.println(cli.toString());
}
void pongCallback(cmd* cmdPtr) {
  Command cmd(cmdPtr);

  int argNum = cmd.countArgs();

  for (int i = 0; i < argNum; i++) {
    Serial.println(cmd.getArgument(i).getValue());
  }
}

void pingCallback(cmd* cmdPtr) {
  Command cmd(cmdPtr);

  Argument argN   = cmd.getArgument("num");
  String   argVal = argN.getValue();
  int n           = argVal.toInt();

  Argument argStr = cmd.getArgument("str");
  String   strVal = argStr.getValue();

  Argument argC = cmd.getArgument("c");
  bool     c    = argC.isSet();

  if (c) strVal.toUpperCase();

  for (int i = 0; i < n; i++) {
    Serial.println(strVal);
  }
}



void convergeCB(cmd* cmdPtr) {
  Command cmd(cmdPtr); // do nothing, required for creating command
  blinker.disable();
  LED_state = false;
  LEDOff();
  runner.addTask(converger);
  converger.enable();
  Serial.println("command accepted");
}

void getRPYCB(cmd* cmdPtr) {
  Command cmd(cmdPtr); // get arguments
  Argument argOff   = cmd.getArgument("off");
  Argument argOn    = cmd.getArgument("on");

  Argument argCS    = cmd.getArgument("cs");
  String pin        = argCS.getValue();
  int csPin         = pin.toInt();

  bool sensorOn  = argOn.isSet();
  bool sensorOff  = argOff.isSet();

  if (sensorOn) {
    switch (csPin) {
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
  } else if (sensorOff) {
    switch (csPin) {
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
void getAccelRawCB(cmd* cmdPtr) {
  Command cmd(cmdPtr); // get arguments
  Argument argOff   = cmd.getArgument("off");
  Argument argOn    = cmd.getArgument("on");

  Argument argCS    = cmd.getArgument("cs");
  String pin        = argCS.getValue();
  int csPin         = pin.toInt();

  bool sensorOn  = argOn.isSet();
  bool sensorOff  = argOff.isSet();

  if (sensorOn) {
    switch (csPin) {
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
  } else if (sensorOff) {
    switch (csPin) {
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
void getAccelMssCB(cmd* cmdPtr) {
  Command cmd(cmdPtr); // get arguments
  Argument argOff   = cmd.getArgument("off");
  Argument argOn    = cmd.getArgument("on");

  Argument argCS    = cmd.getArgument("cs");
  String pin        = argCS.getValue();
  int csPin         = pin.toInt();

  bool sensorOn  = argOn.isSet();
  bool sensorOff  = argOff.isSet();

  if (sensorOn) {
    switch (csPin) {
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
  } else if (sensorOff) {
    switch (csPin) {
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
void getMargCB(cmd* cmdPtr) {
  Command cmd(cmdPtr); // get arguments
  Argument argOff   = cmd.getArgument("off");
  Argument argOn    = cmd.getArgument("on");

  Argument argCS    = cmd.getArgument("c");
  String pin        = argCS.getValue();
  int csPin         = pin.toInt();

  bool sensorOn  = argOn.isSet();
  bool sensorOff  = argOff.isSet();

  if (sensorOn) {
    switch (csPin) {
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
  } else if (sensorOff) {
    switch (csPin) {
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

void motorControlCB(cmd* cmdPtr) {
  Command cmd(cmdPtr); // get arguments
  Argument argOn  = cmd.getArgument("e");
  Argument argX   = cmd.getArgument("x");
  Argument argY   = cmd.getArgument("y");
  Argument argZ   = cmd.getArgument("z");
  Argument argAll = cmd.getArgument("all");
  Argument argOff = cmd.getArgument("off");

  Argument amps = cmd.getArgument("a");
  String ampsVal = amps.getValue();
  float driveCurrent = ampsVal.toFloat();

  bool motorEnable = argOn.isSet();
  bool x    = argX.isSet();
  bool y    = argY.isSet();
  bool z    = argZ.isSet();
  bool all  = argAll.isSet();
  bool off  = argOff.isSet();

  if (motorEnable) {
    enableMotor();
    Serial.println("Motors on");
  }
  if (x) {
    float pwm = ampsToPWM(driveCurrent);
    ledcWrite(0, pwm);
    Serial.print(pwm);
    Serial.print(" ");
    Serial.println("x works");
  }
  if (y) {
    float pwm = ampsToPWM(driveCurrent);
    ledcWrite(1, pwm);
    Serial.print(pwm);
    Serial.print(" ");
    Serial.println("y works");
  }
  if (z) {
    float pwm = ampsToPWM(driveCurrent);
    ledcWrite(2, pwm);
    Serial.print(pwm);
    Serial.print(" ");
    Serial.println("z works");
  }
  if (all) {
    float pwm = ampsToPWM(driveCurrent);
    ledcWrite(0, pwm);
    ledcWrite(1, pwm);
    ledcWrite(2, pwm);
    Serial.print(pwm);
    Serial.print(" ");
    Serial.println("all works");
  }
  if (off) {
    ledcWrite(0, nullPWM);
    ledcWrite(1, nullPWM);
    ledcWrite(2, nullPWM);
    disableMotor();
    Serial.println("off works");
  }
}
