#include "Repository.h"

const static int ID_COLUMN = 0;
const static int NAME_COLUMN = 1;
const static int CARD_ID_COLUMN = 2;

int callback(void *data, int argc, char **argv, char **azColName) {
  std::vector<DBCONTENT> *dbcontentList = (std::vector<DBCONTENT> *) data;
  DBCONTENT dbcontent;

  dbcontent.id = (int) argv[ID_COLUMN];
  dbcontent.name = argv[NAME_COLUMN];
  dbcontent.cardId = argv[CARD_ID_COLUMN];

  dbcontentList->push_back(dbcontent);

  return 0;
};

void printSPIFFSFiles()  {
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
}

Repository::Repository(const char *fileName) {
  this->fileName = fileName;

  if (initialize()) open();
  printSPIFFSFiles();
};

int Repository::initialize() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system.");
    return 0;
  }

  sqlite3_initialize();
  Serial.println("SQL LITE Inicialized!");
  return 1;
}

void Repository::open() {
  int rc = sqlite3_open(fileName, &db);
  if (rc) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(db));
    opened = 0;
  } else {
    Serial.println("Open database successfully!\n");
    opened = 1;
  }
};  

int Repository::isOpen() {
  return opened;
}

void Repository::close() {
  sqlite3_close(db);
};

int Repository::exec(const char *sql) {
  Serial.println(sql);

  long start = micros();
  int rc = sqlite3_exec(db, sql, callback, (void*) &dbcontentList, &errMsg);

  if (rc != SQLITE_OK) {
      Serial.printf("SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
  } else {
      Serial.printf("Operation done successfully\n");
  }

  Serial.print(F("Time taken: "));
  Serial.println(micros()-start);
  Serial.println();

  return rc;
};

void Repository::print() {
  for (DBCONTENT content : dbcontentList) {
    Serial.printf("ID: %d, NAME: %s, CARDID: %s\n", content.id, content.name.c_str(), content.cardId.c_str());
  }

  Serial.println();
}