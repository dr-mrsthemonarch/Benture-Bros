/*
class MyStuff { // pick more meaningfull name
    // maybe be more verbose
    using axisvals = std::array<float, 3>;

  private:
    // apply constness to save RAM memory
    static const std::array<float, 3> mag_field_strength = { 38.52F, 37.24F , 38.58F };

    MPU9250 mpu;
    Madgwick filter;
    const std::array<float, 3> mag_offsets;
    const std::array<std::array<float, 3> , 6> mag_softiron_matrix;
    std::array<float, 3> gyros{}; // maybe some internal state?

  public:
    MyStuff(int gpionum, const std::array<float, 3>& mag_offsets, const std::array<std::array<float, 3> , 6>& mag_offsets) :
      mpu{SPI, gpionum},
      filter{some, params, for, filter, constructor},
      mag_softiron_matrix{mag_softiron_matrix},
      mag_offsets{mag_offsets} {
    }


    axisvals calculate_stuff() {
      mpu.readSensor();
      // use const as much as possible
      const std::array<float, 3> gyro = {
        something * mpu.getGyroX_rads(),
        something * mpu.getGyroY_rads(),
        something * mpu.getGyroZ_rads(),
      };
      // ...
      filter.update(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[0][2], mag[0], mag[1], -1 * mag[2]);
      // ...
      return {filter.getRoll(), filter.getPitch(),  filter.getYaw()};
    }
};

std::array<MyStuff, 6> imus = {
  { 21, {10.44F, 34.76F, -49.86F}, {{1.036F,  0.017F,  -0.001F }, {...}, {...} }, // Header P5
    { 25, {....} {{...}, {..}{...} }, // Header P6
      // etc....
    };

    void setup() {
      for (auto && imu : imus) {
        imu.setup();
      }
    }

    void loop() {
      for (auto && imu : imus) {
        const auto&vals =  imu.calculate_stuff();
        for (auto && v : vals) {
          Serial.print(v);
        }
      }
    }

    */
