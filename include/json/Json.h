#include <ArduinoJson.h>

class JSON {
  public:
    StaticJsonDocument<250> jsonDocument;
    virtual void createJson() = 0;
    void clear();
    std::string stringify(const char *response);
};