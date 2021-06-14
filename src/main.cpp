#include <Arduino.h>
#include <sqlite3.h>
#include "CustomServer.h"

const char *apSsid = "esp32-playground";
const char *apPassword = "123456789";

CustomServer server(apSsid, apPassword);

const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}

int dbOpen(const char *fileName, sqlite3 **db) {
  int rc = sqlite3_open(fileName, db);
  if (rc) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
    return 0;
  } else {
    Serial.println("Open database successfully!");
  }

  return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   return rc;
}

void setup() {
  Serial.begin(115200);
  server.init();

  sqlite3 *db;
  int rc;

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system.");
    return;
  }

  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
        Serial.print("  DIR : ");
        Serial.println(file.name());
    } else {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("\tSIZE: ");
        Serial.println(file.size());
    }
    file = root.openNextFile();
  }

  SPIFFS.remove("/test1.db");

  sqlite3_initialize();

  if (dbOpen("/spiffs/test1.db", &db))
    return;

  rc = db_exec(db, "CREATE TABLE test1 (id INTEGER, name VARCHAR(100), cardId VARCHAR(16));");
  rc = db_exec(db, "INSERT INTO test1 VALUES (1, 'Jose da Silveira', '2345682734581234');");
  rc = db_exec(db, "INSERT INTO test1 VALUES (2, 'Jose da Silveira', '2345682734581234');");
  rc = db_exec(db, "SELECT * FROM test1");
  sqlite3_close(db);
}

void loop() {
  server.handleClient();
}