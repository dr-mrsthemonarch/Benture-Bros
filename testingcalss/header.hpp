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

Adafruit_BNO055 bnoSensor = Adafruit_BNO055(-1, 0x28);
using namespace Eigen;

SimpleCLI cli;
//#define betaDef = 10f;
int cs26 = 0;
int cs14 = 1;
int cs21 = 2;
int cs27 = 3;
int cs32 = 4;
int cs25 = 5;
int bno = 6;

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
std::array<std::array<float, 3>, 7> accelValues = {0};
std::array<std::array<float, 3>, 7> accelValuesRaw = {0};
std::array<std::array<float, 3>, 7> accelValuesMss = {0};
std::array<std::array<float, 3>, 7> gyroValues = {0};
std::array<std::array<float, 3>, 7> magValues = {0};
std::array<std::array<float, 3>, 7> rpyValues = {0};
std::array<std::array<float, 3>, 6> offsetValues = {0};

// Class objects
// SensorCaller2 filter(int filter, int gpio, {f ,f ,f }accel,  { f, f,f }mag, {{ {f}, {f}, {f} } }magmatrix)
std::array<SensorCaller2, 6> mpu9250 = {
    SensorCaller2(200, 26, {0.9957, 0.9983, 0.9810, -0.0235, -0.0544, -0.0083}, {4.7, 7.96, -46.23}, {{{0.717, 0.081, -0.023}, {0.081, 1.301, 0.029}, {-0.023, 0.029, 1.082}}}),
    SensorCaller2(200, 14, {0.9980, 0.9978, 0.9785, -0.0057, -0.0306, 0.0392}, {26.77, -10.76, -8.34}, {{{1.135, -0.012, 0.174}, {-0.012, 0.969, 0.011}, {0.174, 0.011, 0.936}}}),
    SensorCaller2(200, 21, {0.9975, 0.9978, 0.9811, -0.0165, -0.0383, 0.0769}, {5.63, 11.97, -38.13}, {{{0.714, -0.109, 0.048}, {-0.109, 1.275, -0.027}, {0.048, -0.027, 1.118}}}),
    SensorCaller2(200, 27, {0.9970, 0.9986, 0.9791, -0.0055, -0.0418, -0.0117}, {35.62, 3.48, -2.13}, {{{1.096, 0.003, -0.119}, {0.003, 1.050, 0.035}, {-0.119, 0.035, 0.884}}}),
    SensorCaller2(200, 32, {0.9964, 0.9976, 0.9739, -0.0080, -0.0375, 0.0835}, {7.78, 27.29, -5.58}, {{{0.861, 0.067, -0.065}, {0.067, 1.255, -0.050}, {-0.065, -0.050, 0.936}}}),
    SensorCaller2(200, 25, {0.9978, 0.9986, 0.9773, -0.0880, -0.1338, -0.1203}, {20.67, 14.87, -23.49}, {{{1.01, 0.013, 0.028}, {0.013, 1.033, -0.009}, {0.028, -0.009, 0.959}}})};

std::array<float, 3> phiRPY; // phi angle displacement
std::array<float, 3> dotPhi; // phi angular velocity
std::array<float, 3> thetaRPY;
std::array<float, 3> dotTheta;
std::array<float, 3> psiRPY;
std::array<float, 3> dotPsi;
std::array<float, 3> lambdaRPY;
std::array<float, 3> dotLambda;
std::array<float, 3> matPhi = {-150, -25, -0.06};   // gain matrix
std::array<float, 3> matTheta = {-150, -25, -0.06}; // gain matrix
std::array<float, 3> matPsi = {-20, -20, -0.02};    // gain matrix
std::array<float, 3> matLambda = {-20,-20,-0.02};   // gain matrix
std::array<float, 3> u;                             // motor input array
std::array<float, 6> magnetic_field = {37.26, 34.86, 35.74, 34.18, 0, 31.86};
std::array<float, 3> omega = {0, 0, 0}; // motor rad/s array
imu::Vector<3> vectorRPY;
imu::Vector<3> vectorGyro;
imu::Vector<3> vectorMag;
imu::Vector<3> vectorAccel;


Vector3f plantPhi(phiRPY[0], phiRPY[1], phiRPY[2]);
Vector3f plantTheta(thetaRPY[0], thetaRPY[1], thetaRPY[2]);
Vector3f plantPsi(psiRPY[0], psiRPY[1], psiRPY[2]);
Vector3f plantLambda(lambdaRPY[0],lambdaRPY[1],lambdaRPY[2]);

Vector3f lqrPhi(matPhi[0], matPhi[1], matPhi[2]);
Vector3f lqrTheta(matTheta[0], matTheta[1], matTheta[2]);
Vector3f lqrPsi(matPsi[0], matPsi[1], matPsi[2]);
Vector3f lqrLambda(matLambda[0],matLambda[1],matLambda[2]);