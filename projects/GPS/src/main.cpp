#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define GPS_TX 16 
#define GPS_RX 17

HardwareSerial mySerial(1);

TinyGPSPlus gps;

String fetchLocation();

void displayInfo();

void setup() {
  Serial.begin(115200); 
  mySerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX); 
  Serial.println("GPS Serial initialized");
}

void loop() {
  String incomingData = fetchLocation();
  Serial.println(incomingData);
  const char* dataPtr = incomingData.c_str();
  while (*dataPtr) {
    if (gps.encode(*dataPtr++)) {
      displayInfo();
    }
  }
  delay(1000);
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
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
    Serial.print(F("UTC"));
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

String fetchLocation(){
  String incomingData = "";
  while (mySerial.available()) {
    char incomingByte = mySerial.read();
    incomingData += incomingByte;
  }
  return incomingData;
}