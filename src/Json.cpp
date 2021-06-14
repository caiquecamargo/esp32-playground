#include "Json.h"

void JSON::stringfy(const char *response) {
  jsonDocument.clear();
  jsonDocument["response"] = response;
  serializeJson(jsonDocument, buffer);
}