#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  mpu.initialize();
  
  Serial.println("Testing MPU6050 connections...");
  Serial.print(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("Accelerometer: ");
  Serial.print("X="); Serial.print(ax); Serial.print(" Y="); Serial.print(ay); Serial.print(" Z="); Serial.println(az);

  Serial.print("Gyroscope: ");
  Serial.print("X="); Serial.print(gx); Serial.print(" Y="); Serial.print(gy); Serial.print(" Z="); Serial.println(gz);

  delay(1000);

  
}
