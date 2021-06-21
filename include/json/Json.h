#include <ArduinoJson.h>

class JSON {
  protected:
    StaticJsonDocument<250> jsonDocument;
    void clear();
    std::string stringify();
};