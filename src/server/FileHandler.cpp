#include "server/FileHandler.h"
#include <sstream>

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

  std::stringstream ss;
  ss << "" << fileSize;

  Log::logS("FileHandler", "Opened File: " + (std::string) fileName + ", Size: " + ss.str());

  file.close();

  webServer.send(200, fileType, buffer);
}