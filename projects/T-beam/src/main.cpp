#include <Arduino.h>

#include "gps.h"
#include "led.h"

void log(void *pvParameters) {
  for (;;) {
    if (!gps::data_is_available()) {
      Serial.println("GPS data is unavailable.");
    } else {
      gps::Data gps_data = gps::get_gps_data();
      Serial.printf("Lat: %.6f Long: %.6f Time: %02d:%02d:%02d\n", gps_data.lat,
                    gps_data.lng, gps_data.time.hours, gps_data.time.minutes,
                    gps_data.time.seconds);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("--- Starting the rocket... ---");
  gps::init();
  xTaskCreate(led::blink_task, "blink", 10000, NULL, 1, NULL);
  xTaskCreate(gps::gps_task, "gps", 10000, NULL, 1, NULL);
  xTaskCreate(log, "log", 10000, NULL, 1, NULL);
}

void loop() {}
