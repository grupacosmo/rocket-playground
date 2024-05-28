#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define GPS_TX 16
#define GPS_RX 17

HardwareSerial mySerial(1);

TinyGPSPlus gps;

struct GPSParams
{
  double latitude;
  double longitude;
  String time;
};

GPSParams decode();

String fetchLocation();

void displayInfo();

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
  Serial.println("GPS Serial initialized");
}

void loop()
{
  String incomingData = fetchLocation();
  Serial.println(incomingData);
  const char *dataPtr = incomingData.c_str();
  while (*dataPtr)
  {
    if (gps.encode(*dataPtr++))
    {
      displayInfo();
      GPSParams params = decode();
      Serial.print("Latitude: ");
      Serial.print(params.latitude, 6);
      Serial.print(" Longitude: ");
      Serial.print(params.longitude, 6);
      Serial.print(" Time: ");
      Serial.println(params.time);
    }
  }
  delay(3000);
}

GPSParams decode()
{
  GPSParams newParams;

  if (gps.location.isValid())
  {
    newParams.latitude = gps.location.lat();
    newParams.longitude = gps.location.lng();
  }
  else
  {
    newParams.latitude = -1;
    newParams.longitude = -1;
  }

  if (gps.time.isValid())
  {
    newParams.time = "";
    uint8_t hour = gps.time.hour();
    uint8_t minute = gps.time.minute();
    uint8_t second = gps.time.second();
    uint8_t centisecond = gps.time.centisecond();

    if (hour < 10)
      newParams.time += "0";
    newParams.time += String(hour) + ":";

    if (minute < 10)
      newParams.time += "0";
    newParams.time += String(minute) + ":";

    if (second < 10)
      newParams.time += "0";
    newParams.time += String(second) + ".";

    if (centisecond < 10)
      newParams.time += "0";
    newParams.time += String(centisecond);

    newParams.time += " UTC";
  }
  else
  {
    newParams.time = "INVALID";
  }

  return newParams;
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
    Serial.print(F(" UTC "));
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

String fetchLocation()
{
  String incomingData = "";
  while (mySerial.available())
  {
    char incomingByte = mySerial.read();
    incomingData += incomingByte;
  }
  return incomingData;
}