#include "mpu.hpp"
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

MPU6050 mpu;

// Zmienne do kalibracji
int16_t ax_offset, ay_offset, az_offset;
int16_t gx_offset, gy_offset, gz_offset;

void mpu_setup() {
  mpu.initialize();
  Serial.println("Testing MPU6050 connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // Resetowanie kalibracji
  ax_offset = ay_offset = az_offset = 0;
  gx_offset = gy_offset = gz_offset = 0;
}

void calibrate() {
  const int calibrationSamples = 100;
  for (int i = 0; i < calibrationSamples; ++i) {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    ax_offset += ax;
    ay_offset += ay;
    az_offset += az;
    gx_offset += gx;
    gy_offset += gy;
    gz_offset += gz;

    delay(10);
  }

  ax_offset /= calibrationSamples;
  ay_offset /= calibrationSamples;
  az_offset /= calibrationSamples;
  gx_offset /= calibrationSamples;
  gy_offset /= calibrationSamples;
  gz_offset /= calibrationSamples;
}

void reset_calibration() {
  ax_offset = ay_offset = az_offset = 0;
  gx_offset = gy_offset = gz_offset = 0;
}

void mpu_loop() {
  static unsigned long lastCalibrationTime = 0;
  static const unsigned long calibrationInterval = 30000; // Kalibracja co 30 sekund

  if (millis() - lastCalibrationTime >= calibrationInterval) {
    //calibrate();
    lastCalibrationTime = millis();
  }

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Rotacja
  ax -= ax_offset;
  ay -= ay_offset;
  az -= az_offset;
  gx -= gx_offset;
  gy -= gy_offset;
  gz -= gz_offset;

  Serial.print("Accelerometer (in m/s^2): ");
  Serial.print("X = "); Serial.print((float)ax / 16384.0 * 9.81);
  Serial.print(", Y = "); Serial.print((float)ay / 16384.0 * 9.81);
  Serial.print(", Z = "); Serial.println((float)az / 16384.0 * 9.81);

  Serial.print("Gyroscope (in degrees per second): ");
  Serial.print("X = "); Serial.print((float)gx / 131.0);
  Serial.print(", Y = "); Serial.print((float)gy / 131.0);
  Serial.print(", Z = "); Serial.println((float)gz / 131.0);

  float acceleration = sqrt(pow((float)ax / 16384.0 * 9.81, 2) + pow((float)ay / 16384.0 * 9.81, 2) + pow((float)az / 16384.0 * 9.81, 2));
  Serial.print("Linear Acceleration (in m/s^2): ");
  Serial.println(acceleration);

  float angular_speed = sqrt(pow((float)gx / 131.0, 2) + pow((float)gy / 131.0, 2) + pow((float)gz / 131.0, 2));
  Serial.print("Angular Speed (in radians per second): ");
  Serial.println(angular_speed);

  delay(5000);
}
