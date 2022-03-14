
void setup() {
  // serial to display data
  Serial.begin(115200);
  while (!Serial) {}
  // start communication with IMU
  /*
    if (!bno.begin()) {
    // There was a problem detecting the BNO055 ... check your connections
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
    }
  */
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  testing[0].setup();
  testing[0].report();
  testing[1].setup();
  testing[1].report();

  startLEDC();

  ///===================================================== SimpleCLI stuff
  //Code failes unless comamnds added within setup()
  cli.setOnError(errorCallback);
  cmdPing = cli.addCommand("ping", pingCallback);
  cmdPing.addPositionalArgument("str", "pong");
  cmdPing.addArgument("n/um/ber,anzahl", "1");
  cmdPing.addFlagArgument("c");

  cmdPong = cli.addBoundlessCommand("pong,hello", pongCallback);

  cmdHelp = cli.addCommand("help", helpCB);
  cmdHelp.setDescription("Lists available commands and flags");

  cmdConverge = cli.addCommand("conv", convergeCB);
  cmdConverge.setDescription("Tests for converagance of sensor n");

  cmdMotor = cli.addCommand("motor", motorControlCB);
  cmdMotor.setDescription("Manually turn motors on or off");
  cmdMotor.addFlagArgument("e/nable,on");
  cmdMotor.addFlagArgument("d/isable,off");
  cmdMotor.addFlagArgument("all");
  cmdMotor.addFlagArgument("x");
  cmdMotor.addFlagArgument("y");
  cmdMotor.addFlagArgument("z");
  cmdMotor.addArgument("a/mps");

  cmdGetAccelRaw = cli.addCommand("getAccelRaw", getAccelRawCB);
  cmdGetAccelRaw.setDescription("Get Raw accelerometer Data");
  cmdGetAccelRaw.addFlagArgument("e/nable,on");
  cmdGetAccelRaw.addFlagArgument("d/isable,off");
  cmdGetAccelRaw.addArgument("c");

  cmdGetAccelMss = cli.addCommand("getAccelMss", getAccelMssCB);
  cmdGetAccelMss.setDescription("Get Meters per Second Second accelerometer Data");
  cmdGetAccelMss.addFlagArgument("e/nable,on");
  cmdGetAccelMss.addFlagArgument("d/isable,off");
  cmdGetAccelMss.addArgument("c");

  cmdGetRPY = cli.addCommand("getRPY", getRPYCB);
  cmdGetRPY.setDescription("Get Meters per Second Second accelerometer Data");
  cmdGetRPY.addFlagArgument("e/nable,on");
  cmdGetRPY.addFlagArgument("d/isable,off");
  cmdGetRPY.addArgument("c");

  cmdGetMarg = cli.addCommand("getMarg", getMargCB);
  cmdGetMarg.setDescription("Get Calibration Data for MotionCal, requires baud 115200");
  cmdGetMarg.addFlagArgument("e/nable,on");
  cmdGetMarg.addFlagArgument("d/isable,off");
  cmdGetMarg.addArgument("c");


  ///===================================================== Taskscheduler stuff
  runner.init();
  //  runner.addTask(probeADC);
  runner.addTask(serialRead);
  runner.addTask(angleCalculateRPY_Rads);
  runner.addTask(blinker);
  runner.addTask(converger);
  angleCalculateRPY_Rads.enable();
  converger.enable();
  serialRead.enable();
  //  probeADC.enable();
}
