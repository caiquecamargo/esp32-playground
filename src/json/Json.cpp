#include "json/Json.h"

std::string JSON::stringify() {  
  std::string buffer;
  serializeJson(jsonDocument, buffer);

  return buffer;
}

void JSON::clear() {
  jsonDocument.clear();
}