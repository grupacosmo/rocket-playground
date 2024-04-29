#include <Arduino.h>
#include <Wire.h>
#include "mpu.hpp"

void setup() {
  Serial.begin(115200);
  Wire.begin(23, 19);
  mpu_setup();
}

void loop() {
  mpu_loop();
}