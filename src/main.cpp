#include <Arduino.h>
#include "server/CustomServer.h"
#include "repository/UserService.h"

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

CustomServer server(apSsid, apPassword);

void setup() {
  Serial.begin(115200);
  server.init();

  UserService db = UserService("/spiffs/test.db");
}

void loop() {
  server.handleClient();
}