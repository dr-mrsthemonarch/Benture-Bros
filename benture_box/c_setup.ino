void setup()
{
  //Start serial
  //Serial.begin(115200);
  Serial.begin(250000);
  while (!Serial) {}
  //  Wire.begin(23, 22);

  //set up chip select pins as outputs as the Arduino API
  //doesn't handle automatically pulling CS low, IMU.begin() sets all CS pins high
  for (int i = 0; i < 5; i++) {
    pinMode(cs[i], OUTPUT);
  }
  // start the mpus
  for (int i = 0; i < 6; i++) {
    imuStatus[i] = emus[i].begin();
  }

  // start communication with IMU set accel and gyro ranges
  for (int i = 0; i < 6; i++) {
    emus[i].setAccelRange(MPU9250::ACCEL_RANGE_2G);
    emus[i].setGyroRange(MPU9250::GYRO_RANGE_250DPS);

  }
/*

    for (int i = 0; i < 6; i++) {
    imuStatus[i] = emus[i].calibrateAccel();
    }
*/

  delay(200); // wait a bit
  //start madgwick filters, may be problems with Madgwick library not producing good results at high sampling rate. > 100
  for (int i = 0; i < 6; i++) {
    filters[i].begin(250);
  }
filter.begin(100);

  for (int i = 0; i < 3; i++) {

    // configure LED PWM functionalitites
    ledcSetup(ledChannel[i], freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(motorPin[i], ledChannel[i]);

    // escon requires 10% PWM signal to activate, set to nullPWM
    ledcWrite(ledChannel[i], nullPWM);

  }

  //start ADS1015 set gain, ranges
  ads.begin();
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  adc1_config_width(ADC_WIDTH_BIT_11);   //Range 0-2048
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //ADC_ATTEN_DB_11 = 0-3,6V

  // initialize Escons as off
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

}
