#include <Arduino.h>
#include "CustomServer.h"
#include "User.h"

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

CustomServer server(apSsid, apPassword);

void setup() {
  Serial.begin(115200);
  server.init();

  User db = User("/spiffs/test3.db");
  db.exec("SELECT * FROM test2");
  // db.exec("CREATE TABLE test2 (id INTEGER, name VARCHAR(100), cardId VARCHAR(16));");
  // db.exec("INSERT INTO test2 VALUES (1, 'Jose da Silveira', '2345682734581234');");
  // db.exec("INSERT INTO test2 VALUES (2, 'Joao Candido', '1345682734581234');");
  // db.exec("SELECT * FROM test2");
  db.printResultSet();
  db.close();
}

void loop() {
  server.handleClient();
}