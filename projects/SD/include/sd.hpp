#pragma once
#include "FS.h"
#include "SD.h"
#include "SPI.h"


void createDir(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void readFile(fs::FS &fs, const char * path);
void writeData(fs::FS &fs, const char * path, const char * message);
