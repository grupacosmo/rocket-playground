#include <Wire.h>
#include <MPU6050.h>
#include <math.h> // Dodane dla funkcji sqrt i atan2

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin(23, 19);
  
  mpu.initialize();
  
  Serial.println("Testing MPU6050 connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Display Accelerometer data
  Serial.print("Akcelerometr (in m/s^2): ");
  Serial.print("X = "); Serial.print((float)ax / 16384.0 * 9.81); // Convert raw data to m/s^2 (1g = 9.81 m/s^2)
  Serial.print(", Y = "); Serial.print((float)ay / 16384.0 * 9.81);
  Serial.print(", Z = "); Serial.println((float)az / 16384.0 * 9.81);

  // Display Gyroscope data
  Serial.print("Zyroskop (in degrees per second): ");
  Serial.print("X = "); Serial.print((float)gx / 131.0); // Convert raw data to degrees per second (sensitivity scale factor)
  Serial.print(", Y = "); Serial.print((float)gy / 131.0);
  Serial.print(", Z = "); Serial.println((float)gz / 131.0);

  // Calculate linear acceleration in m/s^2
  float acceleration = sqrt(pow((float)ax / 16384.0 * 9.81, 2) + pow((float)ay / 16384.0 * 9.81, 2) + pow((float)az / 16384.0 * 9.81, 2));
  Serial.print("Przyspieszenie liniowe (in m/s^2): ");
  Serial.println(acceleration);

  // Calculate angular velocity (speed) in radians/s
  float angular_speed = sqrt(pow((float)gx / 131.0, 2) + pow((float)gy / 131.0, 2) + pow((float)gz / 131.0, 2));
  Serial.print("Predkosc katowa (in radians per second): ");
  Serial.println(angular_speed);

  // Calculate rotation angle in radians
  float rotation_angle = atan2((float)ay / 16384.0, (float)ax / 16384.0);
  Serial.print("Kat obrotu (in radians): ");
  Serial.println(rotation_angle);

  delay(5000);
}
