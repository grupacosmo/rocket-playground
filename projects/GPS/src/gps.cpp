#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <gps.hpp>

#define GPS_TX 16
#define GPS_RX 17

HardwareSerial mySerial(1);

TinyGPSPlus GPSEncoder;

namespace gps
{

  void begin()
  {
    mySerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
  }

  void fetchLocation()
  {
    while (mySerial.available())
    {
      char incomingByte = mySerial.read();
      GPSEncoder.encode(incomingByte);
    }
  }

  Location parseLocation()
  {
    Location newLocation;

    if (GPSEncoder.location.isValid())
    {
      newLocation.latitude = GPSEncoder.location.lat();
      newLocation.longitude = GPSEncoder.location.lng();
    }
    else
    {
      newLocation.latitude = -1;
      newLocation.longitude = -1;
    }

    if (GPSEncoder.time.isValid())
    {
      newLocation.time = "";
      uint8_t hour = GPSEncoder.time.hour();
      uint8_t minute = GPSEncoder.time.minute();
      uint8_t second = GPSEncoder.time.second();
      uint8_t centisecond = GPSEncoder.time.centisecond();

      if (hour < 10)
        newLocation.time += "0";
      newLocation.time += String(hour) + ":";

      if (minute < 10)
        newLocation.time += "0";
      newLocation.time += String(minute) + ":";

      if (second < 10)
        newLocation.time += "0";
      newLocation.time += String(second) + ".";

      if (centisecond < 10)
        newLocation.time += "0";
      newLocation.time += String(centisecond);

      newLocation.time += " UTC";
    }
    else
    {
      newLocation.time = "INVALID";
    }

    return newLocation;
  }

  void printLocation(Location location)
  {
    Serial.print("Latitude: ");
    Serial.print(location.latitude, 6);
    Serial.print(" Longitude: ");
    Serial.print(location.longitude, 6);
    Serial.print(" Time: ");
    Serial.println(location.time);
  }
}