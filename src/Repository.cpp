#include "Repository.h"

const static String CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS %s (card_id PRIMARY KEY, name);";

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

template <typename T>
Repository<T>::Repository(const char *fileName, const char *tableName) {
  this->fileName = fileName;
  this->tableName = tableName;

  if (initialize()) {
    open();
    createTableIfNotExists();
  }
  printSPIFFSFiles();
};

template <typename T>
int Repository<T>::initialize() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system.");
    return 0;
  }

  sqlite3_initialize();
  Serial.println("SQL LITE Inicialized!");
  return 1;
}

template <typename T>
void Repository<T>::open() {
  int rc = sqlite3_open(fileName, &db);
  if (rc) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(db));
    opened = 0;
  } else {
    Serial.println("Open database successfully!\n");
    opened = 1;
  }
};  

template <typename T>
int Repository<T>::isOpen() {
  return opened;
}

template <typename T>
void Repository<T>::close() {
  sqlite3_close(db);
};

template <typename T>
void Repository<T>::cleanResultSet() {
  resultSet.clear();
}

template <typename T>
int Repository<T>::callback(void *data, int argc, char **argv, char **azColName) {
  std::vector<T> *resultSet = (std::vector<T> *) data;
  T dbcontent;

  toObject(&dbcontent, argv);

  resultSet->push_back(dbcontent);

  return 0;
};

template <typename T>
int Repository<T>::exec(const char *sql) {
  Serial.println(sql);

  cleanResultSet();
  long start = micros();
  int rc = sqlite3_exec(db, sql, callback, (void*) &resultSet, &errMsg);

  if (rc != SQLITE_OK) {
      Serial.printf("SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
  } else {
      Serial.printf("Operation done successfully\n");
  }

  Serial.print(F("Time taken: "));
  Serial.println(micros()-start);
  Serial.println();
  printResultSet();

  return rc;
};

template <typename T>
int Repository<T>::createTableIfNotExists() {
  char sql[CREATE_TABLE_SQL.length()];
  sprintf(sql, CREATE_TABLE_SQL.c_str(), tableName);

  return exec(sql);
};