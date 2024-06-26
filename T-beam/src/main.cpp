#include <Arduino.h>
#include <Wire.h>

#include "bmp.h"
#include "constants.h"
#include "gps.h"
#include "led.h"
#include "memory.h"

void setup() {
  Wire.begin(SDA, SCL);
  Serial.begin(BAUD_RATE);
  Serial.println("--- ROCKET COMPUTER START ---");

  memory::init();  // loads config as well
  gps::init();
  bmp::init();

  // xTaskCreate(gps::gps_loop, "gps", 10000, NULL, 1, NULL);
  xTaskCreate(led::blink_task, "blink", 10000, NULL, 1, NULL);
  xTaskCreate(bmp::get_bmp, "bmp", 10000, NULL, 1, NULL);
  xTaskCreate(bmp::print_data, "bmp printing", 10000, NULL, 1, NULL);

  memory::print_data();
  memory::config = memory::Config{222, 456.78, "Hello, EEPROM2!"};
  memory::save_config(memory::config);
}

void loop() {}
