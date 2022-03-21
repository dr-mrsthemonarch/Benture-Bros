#include "SensorCaller2.h"

// https://stackoverflow.com/a/28734794/15907933
struct SensorCaller2::Private
{
  // Private functions
  static float convertToPitch(float t13, float t33)
  {
    return _radToDeg * atan2(-t13, t33);
  }
  static float convertToRoll(float t23)
  {
    return _radToDeg * asin(-t23); // y;
  }
  static float convertToYaw(float t21, float t22)
  {
    return _radToDeg * atan2(-t21, t22); // z
  }
  /*
  Directional cosinus matrix elements
  {
    {0  , 0  , t13},
    {t21, t22, t23},
    {0  , 0  , t33}
  }
  */
  static float t13(float q0, float q1, float q2, float q3)
  {
    return 2 * (q1 * q3 - q0 * q2);
  }
  static float t21(float q1, float q3, float q0, float q2)
  {
    return 2 * (q1 * q2 - q0 * q3);
  }
  static float t22(float q0, float q2)
  {
    return 2 * (q0 * q0 + q2 * q2) - 1;
  }
  static float t23(float q0, float q1, float q2, float q3)
  {
    return 2 * (q2 * q3 + q0 * q1);
  }
  static float t33(float q0, float q3)
  {
    return 2 * (q0 * q0 + q3 * q3) - 1;
  }
};

// thing to call class object, requires GPIO, mag_offset array {x,y,z}, and _magSoftironMatrix {{1,2,3},{4,5,6},{7,8,9}}

SensorCaller2::SensorCaller2(const int filterRate, const int gpio, const std::array<float, 6> &accelOffsets, const std::array<float, 3> &magOffsets, const std::array<std::array<float, 3>, 3> &magSoftironMatrix)
    : sensors(SPI, gpio),
      _accelOffsets(accelOffsets),
      _magOffsets(magOffsets),
      _magSoftironMatrix(magSoftironMatrix),
      _gp(gpio),
      _filterRate(filterRate)
{
}

void SensorCaller2::setup()
{
  // do some setuping stuff
  int status;
  filter.begin(_filterRate); // works best with mpu9250 and beta value 0.09 in magwick library
  status = sensors.begin();
  sensors.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  sensors.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  // sensors.calibrateAccel(); // may not be required, built in functions from bolder flight
  // sensors.calibrateGyro(); // same as above
  delay(20);
}

std::array<float, 3> SensorCaller2::calculateEuler()
{
  sensors.readSensor();
  // use const as much as possible
  //  const  std::array<float, 3> _magento;
  _accel = {
      sensors.getAccelX_mss(),
      sensors.getAccelY_mss(),
      sensors.getAccelZ_mss()};
  _accelCalibrated = {
      _accel[0] * _accelOffsets[0] + _accelOffsets[3],
      _accel[1] * _accelOffsets[1] + _accelOffsets[4],
      _accel[2] * _accelOffsets[2] + _accelOffsets[5]};
  _accelRaw = {
      sensors.getAccelX_raw(),
      sensors.getAccelY_raw(),
      sensors.getAccelZ_raw()};
  _gyro = {//
           sensors.getGyroX_rads(),
           sensors.getGyroY_rads(),
           sensors.getGyroZ_rads()};
  _mag = {
      sensors.getMagX_uT(),
      sensors.getMagY_uT(),
      sensors.getMagZ_uT()};
  _magneto = {
      _mag[0] - _magOffsets[0],
      _mag[1] - _magOffsets[1],
      _mag[2] - _magOffsets[2]};
  float _mx = _magneto[0] * _magSoftironMatrix[0][0] + _magneto[1] * _magSoftironMatrix[0][1] + _magneto[2] * _magSoftironMatrix[0][2];
  float _my = _magneto[0] * _magSoftironMatrix[1][0] + _magneto[1] * _magSoftironMatrix[1][1] + _magneto[2] * _magSoftironMatrix[1][2];
  float _mz = _magneto[0] * _magSoftironMatrix[2][0] + _magneto[1] * _magSoftironMatrix[2][1] + _magneto[2] * _magSoftironMatrix[2][2];

  filter.update(_gyro[0], _gyro[1], _gyro[2], _accel[0], _accel[1], _accel[2], _mx, _my, _mz);
  filter.getQuaternion(&_q[0], &_q[1], &_q[2], &_q[3]); // euler angles may be too unstable consider quaternions

  _dcm[0] = Private::t13(_q[0], _q[1], _q[2], _q[3]); // calculate Richtungs cosinus matrix elemente
  _dcm[1] = Private::t21(_q[0], _q[1], _q[2], _q[3]);
  _dcm[2] = Private::t22(_q[0], _q[2]);
  _dcm[3] = Private::t23(_q[0], _q[1], _q[2], _q[3]);
  _dcm[4] = Private::t33(_q[0], _q[3]);

  float _alpha = Private::convertToRoll(_dcm[3]) * _degToRad;
  float _beta = Private::convertToPitch(_dcm[0], _dcm[4]) * _degToRad;
  float _gamma = Private::convertToYaw(_dcm[1], _dcm[2]) * _degToRad;
  return {_alpha, _beta, _gamma};
}

std::array<float, 3> SensorCaller2::calculateRPY()
{
  sensors.readSensor();
  // use const as much as possible
  //  const  std::array<float, 3> _magento;
  _accel = {
      sensors.getAccelX_mss() - _accelOffsets[0],
      sensors.getAccelY_mss() - _accelOffsets[1],
      sensors.getAccelZ_mss() - _accelOffsets[2]};
  _gyro = {
      sensors.getGyroX_rads(),
      sensors.getGyroY_rads(),
      sensors.getGyroZ_rads()};
  _magneto = {
      sensors.getMagX_uT() - _magOffsets[0],
      sensors.getMagY_uT() - _magOffsets[1],
      sensors.getMagZ_uT() - _magOffsets[2]};
  float _mx = _magneto[0] * _magSoftironMatrix[0][0] + _magneto[1] * _magSoftironMatrix[0][1] + _magneto[2] * _magSoftironMatrix[0][2];
  float _my = _magneto[0] * _magSoftironMatrix[1][0] + _magneto[1] * _magSoftironMatrix[1][1] + _magneto[2] * _magSoftironMatrix[1][2];
  float _mz = _magneto[0] * _magSoftironMatrix[2][0] + _magneto[1] * _magSoftironMatrix[2][1] + _magneto[2] * _magSoftironMatrix[2][2];

  filter.update(_gyro[0], _gyro[1], _gyro[2], _accel[0], _accel[1], _accel[2], _mx, _my, _mz);
  _roll = filter.getRoll();
  _pitch = filter.getPitch();
  _yaw = filter.getYaw();
  return {_roll, _pitch, _yaw};
}
std::array<float, 3> SensorCaller2::calculateRPYRadians()
{
  sensors.readSensor();
  // use const as much as possible
  //  const  std::array<float, 3> _magento;
  _accel = {
      sensors.getAccelX_mss(),
      sensors.getAccelY_mss(),
      sensors.getAccelZ_mss()};
  _accelCalibrated = {
      _accel[0] * _accelOffsets[0] + _accelOffsets[3],
      _accel[1] * _accelOffsets[1] + _accelOffsets[4],
      _accel[2] * _accelOffsets[2] + _accelOffsets[5]};
  _accelRaw = {
      sensors.getAccelX_raw(),
      sensors.getAccelY_raw(),
      sensors.getAccelZ_raw()};
  _gyro = {//
           sensors.getGyroX_rads(),
           sensors.getGyroY_rads(),
           sensors.getGyroZ_rads()};
  _mag = {
      sensors.getMagX_uT(),
      sensors.getMagY_uT(),
      sensors.getMagZ_uT()};
  _magneto = {
      _mag[0] - _magOffsets[0],
      _mag[1] - _magOffsets[1],
      _mag[2] - _magOffsets[2]};
  float _mx = _magneto[0] * _magSoftironMatrix[0][0] + _magneto[1] * _magSoftironMatrix[0][1] + _magneto[2] * _magSoftironMatrix[0][2];
  float _my = _magneto[0] * _magSoftironMatrix[1][0] + _magneto[1] * _magSoftironMatrix[1][1] + _magneto[2] * _magSoftironMatrix[1][2];
  float _mz = _magneto[0] * _magSoftironMatrix[2][0] + _magneto[1] * _magSoftironMatrix[2][1] + _magneto[2] * _magSoftironMatrix[2][2];

  filter.update(_gyro[0], _gyro[1], _gyro[2], _accelCalibrated[0], _accelCalibrated[1], _accelCalibrated[2], _mx, _my, _mz);
  _roll = filter.getRollRadians();
  _pitch = filter.getPitchRadians();
  _yaw = filter.getYawRadians();
  return {_roll, _pitch, _yaw};
}

void SensorCaller2::report()
{
  Serial.print("Sensor: ");
  Serial.print(_gp);
  Serial.println(" Started");
}
std::array<float, 3> SensorCaller2::getAccelMss()
{
  return _accel;
}
std::array<float, 3> SensorCaller2::getAccelRaw()
{
  return _accelRaw;
}
float SensorCaller2::getXAccelRaw()
{
  return _accelRaw[0];
}
float SensorCaller2::getYAccelRaw()
{
  return _accelRaw[1];
}
float SensorCaller2::getZAccelRaw()
{
  return _accelRaw[2];
}
std::array<float, 3> SensorCaller2::getGyro()
{
  std::array<float, 3> _swapped;
  _swapped = {_gyro[0],  _gyro[2], _gyro[1]}; // gyro[0] = roll, gyro[2] = pitch, gyro[1] = yaw, gyro[1] *-1 to give same direction rotation
  return _swapped;
}
float SensorCaller2::getXGyro()
{
  return _gyro[0];
}

float SensorCaller2::getYGyro()
{
  return _gyro[1];
}

float SensorCaller2::getZGyro()
{
  return _gyro[2];
}
std::array<float, 3> SensorCaller2::getMag()
{
  return _mag;
}
float SensorCaller2::_dir = 0;

bool SensorCaller2::converge(float value, float range, float dt)
{
  float currVal, sol;
  static float oldVal = 0;
  currVal = value;
  sol = (currVal - oldVal) * 1 / dt;
  if (sol > -1 * range && sol < range)
  {
    _dir = sol;
    oldVal = currVal;
    return true;
  }
  else
  {
    _dir = sol;
    oldVal = currVal;
    return false;
  }
}
float SensorCaller2::getDT()
{
  return SensorCaller2::_dir;
}