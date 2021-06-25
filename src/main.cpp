#include <Arduino.h>
#include "SPIFFS.h"
#include "server/CustomServer.h"
#include "log/Log.h"
#include <sstream>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

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
  // configureDateTime();

  if (!SPIFFS.begin()) {
    Log::logS("SPIFFS", "Failed to mount file system.");
  }

  printSPIFFSFiles();

  server.init();
}

void loop() {
  // server.handleClient();
  vTaskDelay(10);
}