#include <Arduino.h>
#include "SPIFFS.h"
#include "server/CustomServer.h"
#include "log/Log.h"
#include <sstream>
#include <SPI.h>
#include <MFRC522.h>
#include "rfid/RFID.h"
#include "accessController/AccessController.h"

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

// #define SS_PIN 21
// #define RST_PIN 22
// MFRC522 mfrc522(SS_PIN, RST_PIN);

CustomServer server(apSsid, apPassword);

void printSPIFFSFiles()  {
  File root = SPIFFS.open("/");
  if (!root) {
    Log::logS("SPIFFS", "Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Log::logS("SPIFFS", "Mot a directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Log::logS("SPIFFS", "DIR: " + (std::string) file.name());
    } else {
      std::stringstream ss;
      ss << "" << file.size();
      Log::logS("SPIFFS", "FILE: " + (std::string) file.name() + " - Size: " + ss.str());
    }
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);
  
  if (!SPIFFS.begin()) {
    Log::logS("SPIFFS", "Failed to mount file system.");
  }

  printSPIFFSFiles();

  SPI.begin();
  // mfrc522.PCD_Init();
  // RFID rfid = RFID(mfrc522);
  AccessController accessController;
  accessController.init();

  // server.init();
}

void loop() {
}