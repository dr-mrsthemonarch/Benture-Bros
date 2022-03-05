//Required Libraries

#include <Wire.h>
#include <Adafruit_ADS1X15.h> //External ADC
#include <driver/adc.h>
#include "MPU9250.h" //Bolder Flight MPU9250 Library
#include <Madgwick.h> //Madgwick filter modifed adafruit lib 1.1.3, works better than newest version! Gyro in RAD/S if things are wonky. 
//#include <esp_now.h>
//#include <WiFi.h>
//Code used for test bench platform, NOT FOR USE WITH BENTURE BOX
