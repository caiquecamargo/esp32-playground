#include <Arduino.h>
#include "CustomServer.h"
#include "User.h"

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

CustomServer server(apSsid, apPassword);

void setup() {
  Serial.begin(115200);
  server.init();

  User db = User("/spiffs/test.db");
  DB_USER data;
  data.cardId = "123456789";
  data.name = "João das neves";

  DB_USER data2;
  data2.cardId = "13156498";
  data2.name = "Maria das Graças";

  db.create(data);
  // db.create(data2);
  // db.findAll();
  // db.deleteItem(data.cardId);
  // db.deleteItem(data2.cardId);
  // db.findAll();
  // db.close();
}

void loop() {
  server.handleClient();
}