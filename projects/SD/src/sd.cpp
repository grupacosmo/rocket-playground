#include "sd.hpp"

namespace sd
{

  void writeFile(fs::FS &fs, const char *path, const char *message)
  {
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);

    if (!file)
    {
      Serial.println("Failed to open file for writing");
      return;
    }

    if (file.println(message))
    {
      Serial.println("File written");
    }
    else
    {
      Serial.println("Write failed");
    }
    file.close();
  }

  void createDir(fs::FS &fs, const char *path)
  {
    Serial.printf("Creating Dir: %s\n", path);

    if (fs.mkdir(path))
    {
      Serial.println("Dir created");
    }
    else
    {
      Serial.println("mkdir failed");
    }
  }

  void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
  {
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
      Serial.println("Failed to open directory");
      return;
    }
    if (!root.isDirectory())
    {
      Serial.println("Not a directory");
      return;
    }

    File file = root.openNextFile();
    while (file)
    {
      if (file.isDirectory())
      {
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if (levels)
        {
          listDir(fs, file.name(), levels - 1);
        }
      }
      else
      {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("  SIZE: ");
        Serial.println(file.size());
      }
      file = root.openNextFile();
    }
  }

  void readFile(fs::FS &fs, const char *path)
  {
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
      Serial.println("Failed to open file for reading");
      return;
    }

    Serial.print("Read from file: \n");
    while (file.available())
    {
      Serial.write(file.read());
    }
    file.close();
  }

  void writeData(fs::FS &fs, const char *path, const char *message)
  {
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
      Serial.println("Failed to open file for writing");
      return;
    }
    if (file.println(message))
    {
      Serial.println("File written");
    }
    else
    {
      Serial.println("Write failed");
    }
    file.close();
  }
  void setup()
  {
    if (!SD.begin(5))
    {
      Serial.println("Card Mount Failed");
      return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
      Serial.println("No SD card attached");
      return;
    }
  }
}