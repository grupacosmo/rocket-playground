#ifndef MPU_HPP
#define MPU_HPP

#include <Arduino.h>

struct MPUData {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
};

void mpu_setup();
void mpu_loop();
void reset_mpu();
void calibrate_mpu();
void rotate_mpu(float angle);

#endif
