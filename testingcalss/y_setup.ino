
void setup()
{
  // serial to display data
  Serial.begin(115200);
  while (!Serial)
  {
  }
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

  // start ADS1015 set gain, ranges
  ads.begin();
  ads.setGain(GAIN_ONE);                                      // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  adc1_config_width(ADC_WIDTH_BIT_11);                        // Range 0-2048
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // ADC_ATTEN_DB_11 = 0-3,6V

  // initialize Escons as off
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  ///===================================================== SimpleCLI stuff
  // Code failes unless comamnds added within setup()
  cli.setOnError(errorCallback);

  cmdHelp = cli.addCommand("help", helpCB);
  cmdHelp.setDescription("Lists available commands and flags");

  cmdConverge = cli.addCommand("conv", convergeCB);
  cmdConverge.setDescription("Tests for converagance of sensor n");

  cmdMotorState = cli.addCommand("motorstate", motorStateCB);
  cmdMotorState.setDescription("Turn motors on or off");
  cmdMotorState.addFlagArgument("e/nable,on");
  cmdMotorState.addFlagArgument("d/isable,off");

  cmdMotor = cli.addCommand("motor", motorControlCB);
  cmdMotor.setDescription("Manually set motor current");
  cmdMotor.addArgument("a/mps");
  cmdMotor.addArgument("p");

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
  cmdGetRPY.setDescription("Get Roll, Pitch, and Yaw Data in Radians");
  cmdGetRPY.addFlagArgument("e/nable,on");
  cmdGetRPY.addFlagArgument("d/isable,off");
  cmdGetRPY.addArgument("c");

  cmdGetGyro = cli.addCommand("getGyro", getGyroCB);
  cmdGetGyro.setDescription("Get Gyro Data in Roll, Pitch, Yaw in Rad/s");
  cmdGetGyro.addFlagArgument("e/nable,on");
  cmdGetGyro.addFlagArgument("d/isable,off");
  cmdGetGyro.addArgument("c");

  cmdGetMarg = cli.addCommand("getMarg", getMargCB);
  cmdGetMarg.setDescription("Get Calibration Data for MotionCal, requires baud 115200");
  cmdGetMarg.addFlagArgument("e/nable,on");
  cmdGetMarg.addFlagArgument("d/isable,off");
  cmdGetMarg.addArgument("c");

  cmdGetPlant = cli.addCommand("getPlant", getPlantCB);
  cmdGetPlant.setDescription("Get Pi Subsystem SIMO Data, Rad_i, Rad_i/s, Omega_i");
  cmdGetPlant.addFlagArgument("e/nable,on");
  cmdGetPlant.addFlagArgument("d/isable,off");
  cmdGetPlant.addArgument("s");

  cmdLQR = cli.addCommand("lqr", lqrCB);
  cmdLQR.setDescription("Turn on LQR Controllers, print Controller Output, set Gain values");
  cmdLQR.addFlagArgument("e/nable,on");
  cmdLQR.addFlagArgument("d/isable,off");
  cmdLQR.addFlagArgument("c");
  cmdLQR.addFlagArgument("p/rint");

  modLQR = cli.addCommand("lqrSet", lqrSetCB);
  modLQR.setDescription("Modify lqr gains for subsystem p_i");
  modLQR.addArgument("k1");
  modLQR.addArgument("k2");
  modLQR.addArgument("k3");
  modLQR.addArgument("p");

  cmdDebug = cli.addCommand("debug", debugCB);
  cmdDebug.setDescription("Print various debug infos to Serial");
  cmdDebug.addFlagArgument("e/nable,on");
  cmdDebug.addFlagArgument("d/isable,off");

  ///===================================================== Taskscheduler stuff
  runner.init();

  runner.addTask(serialRead);
  runner.addTask(blinker);
  runner.addTask(converger);

  //  runner.addTask(probeADC);
  runner.addTask(angleCalculateRPY_Rads);
  runner.addTask(plantCalculate);
  runner.addTask(motorDriver);
  runner.addTask(controller);

  angleCalculateRPY_Rads.enable();
//  converger.enable();
  plantCalculate.enable();
  controller.enable();
  serialRead.enable();
  probeADC.enable();
}
