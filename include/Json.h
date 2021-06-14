#include <ArduinoJson.h>

class JSON {
  public:
    StaticJsonDocument<250> jsonDocument;
    char buffer[250];
    void stringfy(const char *response);
};