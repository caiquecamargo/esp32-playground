#include "SPIFFS.h"
#include <WebServer.h>
#include "log/Log.h"

class FileHandler {
  public:
    static void sendFile(const char* fileName, const char* fileType, WebServer& webServer);
};