#include "Json.h"

void JSON::stringify(const char *response) {
  jsonDocument.clear();
  jsonDocument["response"] = response;
  serializeJson(jsonDocument, buffer);
}