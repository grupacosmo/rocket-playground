#include <Arduino.h>
#include <Wire.h>
#include "bmp.hpp"



void pseudo_setup() {
  Wire.begin(23,19);
  Serial.begin(9600);
  bmp::begin();
}


void setup() {
  pseudo_setup();
}

void loop() {
  bmp::pretty_print();
}