#include "server/FileHandler.h"

void FileHandler::sendFile(const char* fileName, const char* fileType, WebServer& webServer) {
  if(!SPIFFS.begin()) {
    Serial.println("An error has ocurred while mounting SPIFFS");
    return;
  }

  Serial.print("Reading file ");
  Serial.println(fileName);
  File file = SPIFFS.open(fileName);
  if (!file) {
    Serial.println("Failed to open file for read");
    return;
  }

  size_t fileSize = file.size();
  char buffer[fileSize];
  file.read((uint8_t *)buffer, fileSize);
  buffer[fileSize] = '\0';

  file.close();

  webServer.send(200, fileType, buffer);
}