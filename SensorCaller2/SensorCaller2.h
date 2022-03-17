#ifndef SensorCaller2_h
#define SensorCaller2_h
#include <array>
#include <Arduino.h>
#include <MPU9250.h>
#include <Madgwick.h>

class SensorCaller2
{
    using axisval = std::array<float, 3>;

private:
    // apply constness to save RAM memory
    // initialize outside classes
    MPU9250 sensors;
    Madgwick filter;

    // initialize private arrays/vars
    const std::array<float, 2> _magFieldStrength = {{41.85F, 40.6F}};
    const std::array<float, 3> _accelOffsets;
    const std::array<float, 3> _gyroOffsets = {{0}};
    const std::array<float, 3> _magOffsets;
    const std::array<std::array<float, 3>, 3> _magSoftironMatrix;
    std::array<float, 4> _q = {{0}};
    std::array<float, 5> _dcm = {{0}}; // init array to save calculated rcm values
    struct Private;
    static constexpr float _radToDeg = 57.296;
    int _gp;
    int _filterRate;
    float _roll;
    float _pitch;
    float _yaw;
    std::array<float, 3> _accel;
    std::array<float, 3> _accelCalibrated;
    std::array<float, 3> _accelRaw;
    std::array<float, 3> _gyro;
    std::array<float, 3> _mag;
    std::array<float, 3> _magneto;
    static float _dir;

public:
    SensorCaller2(const int filterRate, const int gpio, const std::array<float, 3> &accelOffsets, const std::array<float, 3> &magOffsets, const std::array<std::array<float, 3>, 3> &magSoftironMatrix);
    void setup();
    axisval calculateEuler();
    axisval calculateRPY();
    axisval calculateRPYRadians();
    void report();
    axisval getGyro();
    axisval getAccelRaw();
    axisval getAccelMss();
    axisval getMag();
    float getXAccelRaw();
    float getYAccelRaw();
    float getZAccelRaw();
    float getXGyro();
    float getYGyro();
    float getZGyro();
    bool converge(float value, float range, float dt);
    float getDT();
};
#endif