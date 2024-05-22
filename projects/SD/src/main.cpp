#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "sd.hpp"

void setup(){
  Serial.begin(9600);
  sd::setup();
  sd::writeFile(SD, "/mydir/hello.txt", "siemano4");
  delay(1000);
  sd::readFile(SD, "/mydir/hello.txt");
}

void loop(){

}