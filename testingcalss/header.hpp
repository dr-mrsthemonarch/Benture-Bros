#include <array>
#include <Arduino.h>
#include <Eigen.h>
//#include <utility/imumaths.h>
#include <TaskScheduler.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <driver/adc.h>
#include <SensorCaller2.h>
#include <SimpleCLI.h>

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

//===============================================================motor stuff

int16_t adc[3]; // ADS1015
int motorPin[3] = {15, 12, 33}; // in order x, y, z motors
int enablePin = 13;
int lowPWM = 6554; // 10% from 65535 (16bit PWM)
int highPWM = 58982; // 90% from 65535 (16bit PWM)
int nullPWM = (highPWM + lowPWM) / 2; // Zero PWM for escons
float ampPWM = nullPWM;
// set ledc params
int freq = 1200; // PWM freq
int16_t ledChannel[3] = {0, 1, 2}; // PWM channels
int resolution = 16; // in bits

//===============================================================motor stuff

String  value, value1, value2, switcher, chipselect = "0";
String stringRead;

std::array<std::array<float, 3> , 3> LQRController = {0};
std::array<std::array<float, 3> , 2> accelValues = {0};
std::array<std::array<float, 3> , 2> accelValuesRaw = {0};
std::array<std::array<float, 3> , 2> accelValuesMss = {0};
std::array<std::array<float, 3> , 2> gyroValues = {0};
std::array<std::array<float, 3> , 2> magValues = {0};
std::array<std::array<float, 3> , 2> rpyValues = {0};

//Class objects
std::array<SensorCaller2, 2> testing = {
  SensorCaller2(250, 26, {0.0478, 0.0328, 0.2179}, {10.58, 33.41, -51.91} , {{{ 0.996, 0.015, -0.001 },  -0.014, 0.998, 0.014 , { -0.001, 0.015, 1.007 }}}),
  SensorCaller2(250, 14, {0.0478, 0.0328, 0.2179}, { -10.33, 18.06, -5.73}, {{{ 1.011, -0.029, -0.005 }, { -0.029, 1.013, 0.029 }, { -0.005, 0.029, 0.978 }}})
};

std::array<float, 3> phiRPY; //phi angle displacement
std::array<float, 3> dotPhi; //phi angular velocity
std::array<float, 3> thetaRPY;
std::array<float, 3> dotTheta;
std::array<float, 3> psiRPY;
std::array<float, 3> dotPsi;
std::array<float, 3> matPhi = {-150,-25,-0.06}; // gain matrix
std::array<float, 3> matTheta = {-150,-25,-0.06}; //gain matrix
std::array<float, 3> matPsi = {-20,-20,-0.02}; //gain matrix
std::array<float, 3> u; // motor input array
std::array<float, 6> magnetic_field = {39.09, 37.16, 0,0,0,0};
std::array<float, 3> omega = {0, 0, 0}; //motor rad/s array

Vector3f plantPhi(phiRPY[0], phiRPY[1], phiRPY[2]);
Vector3f plantTheta(thetaRPY[0], thetaRPY[1], thetaRPY[2]);
Vector3f plantPsi(psiRPY[0], psiRPY[1], psiRPY[2]);

Vector3f lqrPhi(matPhi[0], matPhi[1], matPhi[2]);
Vector3f lqrTheta(matTheta[0], matTheta[1], matTheta[2]);
Vector3f lqrPsi(matPsi[0], matPsi[1], matPsi[2]);

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
// SensorCaller2 filter(filter, gpio, accel, mag, magmatrix)
