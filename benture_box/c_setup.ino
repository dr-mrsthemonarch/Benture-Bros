void setup()
{
  //Start serial
  Serial.begin(250000);
  while (!Serial) {}

  // start the mpus
  imuStatus[0] = IMU0.begin();
  imuStatus[1] = IMU1.begin();
  imuStatus[2] = IMU2.begin();
  imuStatus[3] = IMU3.begin();
  imuStatus[4] = IMU4.begin();
  imuStatus[5] = IMU5.begin();

  // start communication with IMU set accel and gyro ranges
  IMU0.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU0.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU1.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU1.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU2.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU2.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU3.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU3.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU4.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU4.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU5.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU5.setGyroRange(MPU9250::GYRO_RANGE_250DPS);

  /*
    imuStatus[0] = IMU0.calibrateAccel();
    imuStatus[1] = IMU1.calibrateAccel();
    imuStatus[2] = IMU2.calibrateAccel();
    imuStatus[3] = IMU3.calibrateAccel();
    imuStatus[4] = IMU4.calibrateAccel();
    imuStatus[5] = IMU5.calibrateAccel();
  */

  delay(200); // wait a bit

  //start madgwick filters
  filter0.begin(300);
  filter1.begin(300);
  filter2.begin(300);
  filter3.begin(300);
  filter4.begin(300);
  filter5.begin(300);

  for (int i = 0; i < 2; i++) {

    // configure LED PWM functionalitites
    ledcSetup(ledChannel[i], freq, resolution);
    
    // attach the channel to the GPIO to be controlled
    ledcAttachPin(motorPin[i], ledChannel[i]);
    
    // escon requires 10% PWM signal to activate, set to nullPWM
    ledcWrite(ledChannel[i], nullPWM);
  }

  /*
    // configure LED PWM functionalitites
    ledcSetup(ledChannel[0], freq, resolution);
    ledcSetup(ledChannel[1], freq, resolution);
    ledcSetup(ledChannel[2], freq, resolution);
    // attach the channel to the GPIO to be controlled
    ledcAttachPin(motorPin[0], ledChannel[0]);
    ledcAttachPin(motorPin[1], ledChannel[1]);
    ledcAttachPin(motorPin[2], ledChannel[2]);
    // escon requires 10% PWM signal to activate, set to nullPWM
    ledcWrite(ledChannel[0], nullPWM);
    ledcWrite(ledChannel[1], nullPWM);
    ledcWrite(ledChannel[2], nullPWM);
  */

  //start ADS1015 set gain, ranges
  ads.begin();
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  adc1_config_width(ADC_WIDTH_BIT_11);   //Range 0-2048
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //ADC_ATTEN_DB_11 = 0-3,6V

  // initialize Escons as off
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling CS low
  for (int i = 0; i < 5; i++) {
    
    pinMode(cs[i], OUTPUT);
    
  }

}
