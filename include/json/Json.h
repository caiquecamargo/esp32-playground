#include <ArduinoJson.h>

class JSON {
  protected:
    StaticJsonDocument<1000> jsonDocument;
    void clear();
    std::string stringify();
};