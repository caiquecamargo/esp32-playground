#include "repository/Repository.h"

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

Repository::Repository(std::string fileName, std::string tableName) {
  this->fileName = fileName;
  this->tableName = tableName;
};

void Repository::init() {
  if (initialize()) {
    open();
    createTableIfNotExists();
  }
}

int Repository::initialize() {
  sqlite3_initialize();
  Serial.println("SQLITE Inicialized!");
  return 1;
}

void Repository::open() {
  int rc = sqlite3_open(fileName.c_str(), &db);
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

int Repository::callback(void *data, int argc, char **argv, char **azColName) {
  std::vector<std::vector<std::string>> *temp = (std::vector<std::vector<std::string>> *) data;

  std::vector<std::string> dataTemp;
  for (int i = 0; i < argc; i++) {
    dataTemp.push_back(argv[i]);
  }
  temp->push_back(dataTemp);

  return 0;
};

int Repository::exec(std::string sql) {
  Serial.println(sql.c_str());

  cleanResultSet();
  long start = micros();
  std::vector<std::vector<std::string>> temp;
  int rc = sqlite3_exec(db, sql.c_str(), Repository::callback, (void*) &temp, &errMsg);

  PopulateResultSet(&temp);

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