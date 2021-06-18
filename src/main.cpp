#include <Arduino.h>
#include "CustomServer.h"
#include "Repository.h"

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

CustomServer server(apSsid, apPassword);

void setup() {
  Serial.begin(115200);
  server.init();

  Repository db = Repository("/spiffs/test.db", "test");
  DB_DATA data;
  data.cardId = "123456789";
  data.name = "João das neves";
  db.create(data);
  db.findById(data.cardId);
  data.name = "João Maranhão";
  db.update(data);
  db.findAll();
  db.deleteItem(data.cardId);
  db.findAll();
  db.close();
}

void loop() {
  server.handleClient();
}