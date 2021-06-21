#include "SPIFFS.h"
#include <WebServer.h>

class FileHandler {
  public:
    static void sendFile(const char* fileName, const char* fileType, WebServer& webServer);
};