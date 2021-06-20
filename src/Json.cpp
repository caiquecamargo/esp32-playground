#include "Json.h"

std::string JSON::stringify(const char *response) {
  clear();
  createJson();
  
  std::string buffer;
  serializeJson(jsonDocument, buffer);

  return buffer;
}

void JSON::clear() {
  jsonDocument.clear();
}