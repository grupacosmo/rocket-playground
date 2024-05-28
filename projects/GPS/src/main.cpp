#include <Arduino.h>
#include <gps.hpp>

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  gps::fetchLocation();
  gps::Location params = gps::parseLocation();
  gps::printLocation(params);

  delay(3000);
}
