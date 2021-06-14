#include <Arduino.h>
#include "CustomServer.h"

#define LED_PIN 2

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

CustomServer server(apSsid, apPassword);

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  server.init();
}

void loop() {
  server.handleClient();
}