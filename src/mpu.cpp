#include "mpu.hpp"
#include <Wire.h>
#include <math.h>

MPU6050 mpu;

int16_t ax_offset, ay_offset, az_offset;
int16_t gx_offset, gy_offset, gz_offset;

void mpu_setup() {
  mpu.initialize();
  Serial.println("Testing MPU6050 connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  reset_mpu(); // Reset kalibracji
  calibrate_mpu(); // Kalibracja przy starcie
}

void reset_mpu() {
  ax_offset = ay_offset = az_offset = 0;
  gx_offset = gy_offset = gz_offset = 0;
}

void calibrate_mpu() {
  const int calibrationSamples = 100; // Mniej próbek dla szybszej kalibracji
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

void rotate_mpu(float angle) {
  float cosAngle = cos(angle);
  float sinAngle = sin(angle);

  // Obliczanie nowych wartości przesunięcia kątowego
  float new_gx_offset = cosAngle * gx_offset - sinAngle * gy_offset;
  float new_gy_offset = sinAngle * gx_offset + cosAngle * gy_offset;
  float new_gz_offset = gz_offset; // Bez zmian w przypadku obrotu wokół osi Z

  // Aktualizacja wartości przesunięcia kątowego
  gx_offset = round(new_gx_offset);
  gy_offset = round(new_gy_offset);
  gz_offset = round(new_gz_offset);
}

void mpu_loop() {
  MPUData data;
  
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  data.ax = (float)(ax - ax_offset) / 16384.0 * 9.81;
  data.ay = (float)(ay - ay_offset) / 16384.0 * 9.81;
  data.az = (float)(az - az_offset) / 16384.0 * 9.81;
  data.gx = (float)(gx - gx_offset) / 131.0;
  data.gy = (float)(gy - gy_offset) / 131.0;
  data.gz = (float)(gz - gz_offset) / 131.0;

  Serial.print("Accelerometer (in m/s^2): ");
  Serial.print("X = "); Serial.print(data.ax);
  Serial.print(", Y = "); Serial.print(data.ay);
  Serial.print(", Z = "); Serial.println(data.az);

  Serial.print("Gyroscope (in degrees per second): ");
  Serial.print("X = "); Serial.print(data.gx);
  Serial.print(", Y = "); Serial.print(data.gy);
  Serial.print(", Z = "); Serial.println(data.gz);

  float acceleration = sqrt(pow(data.ax, 2) + pow(data.ay, 2) + pow(data.az, 2));
  Serial.print("Linear Acceleration (in m/s^2): ");
  Serial.println(acceleration);

  float angular_speed = sqrt(pow(data.gx, 2) + pow(data.gy, 2) + pow(data.gz, 2));
  Serial.print("Angular Speed (in radians per second): ");
  Serial.println(angular_speed);

  delay(5000);
}
