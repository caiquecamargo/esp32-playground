#include "server/FileHandler.h"

void FileHandler::sendFile(const char* fileName, const char* fileType, WebServer& webServer) {
  Log::logS("FileHandler", "Reading file " + (std::string) fileName);
  File file = SPIFFS.open(fileName);
  if (!file) {
    Log::logS("FileHandler", "Failed to open file for read");
    return;
  }

  size_t fileSize = file.size();
  char buffer[fileSize];
  file.read((uint8_t *)buffer, fileSize);
  buffer[fileSize] = '\0';

  Serial.println(buffer);

  file.close();

  webServer.send(200, fileType, buffer);
}