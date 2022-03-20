#include <array>
#include <Arduino.h>
#include <Eigen.h>
#include <TaskScheduler.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <driver/adc.h>
#include <SensorCaller2.h>
#include <SimpleCLI.h>
#include <Adafruit_ADS1X15.h> //External ADC

using namespace Eigen;

SimpleCLI cli;
//#define betaDef = 10f;
int cs26 = 0;
int cs14 = 1;
int cs21 = 2;
int cs27 = 3;
int cs32 = 4;
int cs25 = 5;

std::array<int, 6> cs = {26, 14, 21, 27, 32, 25};
bool LED_state = false;
float g = 0.1019716213;

//===============================================================motor stuff
Adafruit_ADS1015 ads;           /* Use this for the 12-bit version */
int16_t adc[3];                 // ADS1015
int motorPin[3] = {15, 12, 33}; // in order  motors
int enablePin = 13;
int lowPWM = 6554;                    // 10% from 65535 (16bit PWM)
int highPWM = 58982;                  // 90% from 65535 (16bit PWM)
int nullPWM = (highPWM + lowPWM) / 2; // Zero PWM for escons
float ampPWM = nullPWM;
// set ledc params
int freq = 1200;                   // PWM freq
int16_t ledChannel[3] = {0, 1, 2}; // PWM channels
int resolution = 16;               // in bits

//===============================================================motor stuff

String value, value1, value2, switcher, chipselect = "0";
String stringRead;

std::array<std::array<float, 3>, 3> LQRController = {0};
std::array<std::array<float, 3>, 6> accelValues = {0};
std::array<std::array<float, 3>, 6> accelValuesRaw = {0};
std::array<std::array<float, 3>, 6> accelValuesMss = {0};
std::array<std::array<float, 3>, 6> gyroValues = {0};
std::array<std::array<float, 3>, 6> magValues = {0};
std::array<std::array<float, 3>, 6> rpyValues = {0};

// Class objects
// SensorCaller2 filter(int filter, int gpio, {f ,f ,f }accel,  { f, f,f }mag, {{ {f}, {f}, {f} } }magmatrix)
std::array<SensorCaller2, 6> testing = {
    SensorCaller2(250, 26, {0.9957, 0.9983, 0.9810, -0.0235, -0.0544, -0.0083}, {10.58, 33.41, -51.91}, {{{0.996, 0.015, -0.001}, -0.014, 0.998, 0.014, {-0.001, 0.015, 1.007}}}),
    SensorCaller2(250, 14, {0.9980, 0.9978, 0.9785, -0.0057, -0.0306, 0.0392}, {-10.33, 18.06, -5.73}, {{{1.011, -0.029, -0.005}, {-0.029, 1.013, 0.029}, {-0.005, 0.029, 0.978}}}),
    SensorCaller2(250, 21, {0.9975, 0.9978, 0.9811, -0.0165, -0.0383, 0.0769}, {-10.33, 18.06, -5.73}, {{{1.011, -0.029, -0.005}, {-0.029, 1.013, 0.029}, {-0.005, 0.029, 0.978}}}),
    SensorCaller2(250, 27, {0.9970, 0.9986, 0.9791, -0.0055, -0.0418, -0.0117}, {-10.33, 18.06, -5.73}, {{{1.011, -0.029, -0.005}, {-0.029, 1.013, 0.029}, {-0.005, 0.029, 0.978}}}),
    SensorCaller2(250, 32, {0.9964, 0.9976, 0.9739, -0.0080, -0.0375, 0.0835}, {-10.33, 18.06, -5.73}, {{{1.011, -0.029, -0.005}, {-0.029, 1.013, 0.029}, {-0.005, 0.029, 0.978}}}),
    SensorCaller2(250, 25, {0.9978, 0.9986, 0.9773, -0.0880, -0.1338, -0.1203}, {-10.33, 18.06, -5.73}, {{{1.011, -0.029, -0.005}, {-0.029, 1.013, 0.029}, {-0.005, 0.029, 0.978}}})};

std::array<float, 3> phiRPY; // phi angle displacement
std::array<float, 3> dotPhi; // phi angular velocity
std::array<float, 3> thetaRPY;
std::array<float, 3> dotTheta;
std::array<float, 3> psiRPY;
std::array<float, 3> dotPsi;
std::array<float, 3> matPhi = {-150, -25, -0.06};   // gain matrix
std::array<float, 3> matTheta = {-150, -25, -0.06}; // gain matrix
std::array<float, 3> matPsi = {-20, -20, -0.02};    // gain matrix
std::array<float, 3> u;                             // motor input array
std::array<float, 6> magnetic_field = {39.09, 37.16, 0, 0, 0, 0};
std::array<float, 3> omega = {0, 0, 0}; // motor rad/s array

Vector3f plantPhi(phiRPY[0], phiRPY[1], phiRPY[2]);
Vector3f plantTheta(thetaRPY[0], thetaRPY[1], thetaRPY[2]);
Vector3f plantPsi(psiRPY[0], psiRPY[1], psiRPY[2]);

Vector3f lqrPhi(matPhi[0], matPhi[1], matPhi[2]);
Vector3f lqrTheta(matTheta[0], matTheta[1], matTheta[2]);
Vector3f lqrPsi(matPsi[0], matPsi[1], matPsi[2]);

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
