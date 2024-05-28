#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define GPS_TX 16
#define GPS_RX 17

HardwareSerial mySerial(1);

TinyGPSPlus gps;

struct Location
{
  double latitude;
  double longitude;
  String time;
};

void fetchLocation();

Location parseLocation();

void printLocation(Location location);

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
}

void loop()
{
  fetchLocation();
  Location params = parseLocation();
  printLocation(params);

  delay(3000);
}

void fetchLocation()
{
  String locationData = "";
  while (mySerial.available())
  {
    char incomingByte = mySerial.read();
    locationData += incomingByte;
  }
  const char *locationPtr = locationData.c_str();
  while (*locationPtr)
  {
    gps.encode(*locationPtr++);
  }
}

Location parseLocation()
{
  Location newLocation;

  if (gps.location.isValid())
  {
    newLocation.latitude = gps.location.lat();
    newLocation.longitude = gps.location.lng();
  }
  else
  {
    newLocation.latitude = -1;
    newLocation.longitude = -1;
  }

  if (gps.time.isValid())
  {
    newLocation.time = "";
    uint8_t hour = gps.time.hour();
    uint8_t minute = gps.time.minute();
    uint8_t second = gps.time.second();
    uint8_t centisecond = gps.time.centisecond();

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
