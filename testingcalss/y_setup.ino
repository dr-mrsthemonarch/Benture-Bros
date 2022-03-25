
void setup()
{
  // serial to display data
  Serial.begin(115200);
  while (!Serial)
  {
  }

  /* Initialise the sensor */
  if (!bnoSensor.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }
  Serial.print("Bno Started");
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i; i < 6; i++)
  {
    mpu9250[i].setup();
    mpu9250[i].report();
  }
  startLEDC();

  // start ADS1015 set gain, ranges
  ads.begin();
  ads.setGain(GAIN_ONE);                                      // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  adc1_config_width(ADC_WIDTH_BIT_11);                        // Range 0-2048
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // ADC_ATTEN_DB_11 = 0-3,6V

  // initialize Escons as off
  pinMode(enablePin, OUTPUT);   // Set enable pin to IO
  digitalWrite(enablePin, LOW); // Set off for safety
  pinMode(5, OUTPUT);           // not sure what pin 5 is for
  digitalWrite(5, HIGH);        // set high?

  ///===================================================== SimpleCLI stuff
  // Code failes unless comamnds added within setup()
  cli.setOnError(errorCallback);

  cmdHelp = cli.addCommand("help", helpCB);
  cmdHelp.setDescription("Lists available commands and flags");

  cmdConverge = cli.addCommand("conv", convergeCB);
  cmdConverge.setDescription("Tests for converagance of sensor n");

  cmdMotorPrint = cli.addCommand("motorP", motorPrintCB);
  cmdMotorPrint.setDescription("Turn motors on or off, Print Angular Velocities");
  cmdMotorPrint.addFlagArgument("e/nable,on");
  cmdMotorPrint.addFlagArgument("d/isable,off");

  cmdMotorState = cli.addCommand("motorS", motorStateCB);
  cmdMotorState.setDescription("Turn motors on or off, Print Angular Velocities");
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
  cmdGetPlant.addArgument("p");

  cmdLQRPrint = cli.addCommand("lqrPr", lqrPrintCB);
  cmdLQRPrint.setDescription("Print Controller Output");
  cmdLQRPrint.addFlagArgument("e/nable,on");
  cmdLQRPrint.addFlagArgument("d/isable,off");

  cmdprintAll = cli.addCommand("printAll", printAllCB);
  cmdprintAll.setDescription("Print All Output");
  cmdprintAll.addFlagArgument("e/nable,on");
  cmdprintAll.addFlagArgument("d/isable,off");

  cmdLQROn = cli.addCommand("lqrOn", lqrStartCB);
  cmdLQROn.setDescription("Turn on LQR Controllers, 0 All, 1 Phi, 2 Theta, 3 Psi");
  cmdLQROn.addFlagArgument("e/nable,on");
  cmdLQROn.addFlagArgument("d/isable,off");
  cmdLQROn.addFlagArgument("c");
  cmdLQROn.addArgument("s");

  cmdLQR = cli.addCommand("lqr", lqrCB);
  cmdLQR.setDescription("Turn on LQR Controllers, 0 All, 1 Phi, 2 Theta, 3 Psi");
  cmdLQR.addFlagArgument("e/nable,on");
  cmdLQR.addFlagArgument("d/isable,off");
  cmdLQR.addFlagArgument("c");
  cmdLQR.addArgument("s");

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
  //  runner.addTask(angleCalculateRPY_Rads);
  //  runner.addTask(angleCalculateEuler);
  //  runner.addTask(plantCalculate);

  runner.addTask(probeADC);
  runner.addTask(doAll);

  probeADC.enable();
  // angleCalculateEuler.enable();
  // angleCalculateRPY_Rads.enable();
  //  converger.enable();
  //  plantCalculate.enable();
  //  controller.enable();
  doAll.enable();
  serialRead.enable();
}
