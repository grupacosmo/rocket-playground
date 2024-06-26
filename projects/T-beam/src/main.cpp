#include <Arduino.h>
#include <Wire.h>

#include "bmp.h"
#include "gps.h"
#include "led.h"

void parser(void* pvParameters) {
  for (;;) {
    bmp::pretty_print(bmp::global_bmp);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("--- Starting the rocket... ---");
  Wire.begin(21, 22);

  gps::init();
  xTaskCreate(led::blink_task, "blink", 10000, NULL, 1, NULL);
  // xTaskCreate(gps::gps_loop, "gps", 10000, NULL, 1, NULL);

  bmp::init();
  xTaskCreate(bmp::get_bme, "bmp", 10000, NULL, 1, NULL);
  xTaskCreate(parser, "parser", 10000, NULL, 1, NULL);
  xTaskCreate(gps::gps_loop, "gps", 10000, NULL, 1, NULL);
}

void loop() {}
