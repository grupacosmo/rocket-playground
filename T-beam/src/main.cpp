#include <Arduino.h>
#include <Wire.h>

#include "bmp.h"
#include "gps.h"
#include "led.h"
#include "memory.h"

void setup() {
  Serial.begin(115200);
  Serial.println("--- Starting the rocket... ---");
  Wire.begin(21, 22);

  memory::init();  // loads config
  gps::init();
  bmp::init();

  // xTaskCreate(gps::gps_loop, "gps", 10000, NULL, 1, NULL);
  xTaskCreate(led::blink_task, "blink", 10000, NULL, 1, NULL);
  xTaskCreate(bmp::get_bme, "bmp", 10000, NULL, 1, NULL);
  xTaskCreate(bmp::print_data, "bmp printing", 10000, NULL, 1, NULL);

  memory::print_data();
  memory::config = memory::Config{222, 456.78, "Hello, EEPROM2!"};
  memory::save_config(memory::config);
}

void loop() {}
