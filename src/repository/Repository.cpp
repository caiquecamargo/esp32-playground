#include "repository/Repository.h"

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

Repository::Repository(std::string fileName, std::string tableName) : Log("Repository") {
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
  log("SQLITE Inicialized!");
  return 1;
}

void Repository::open() {
  int rc = sqlite3_open(fileName.c_str(), &db);
  if (rc) {
    log("Can't open database: " + (std::string) sqlite3_errmsg(db));
    opened = 0;
  } else {
    log("Open database successfully!");
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
  log("Performing SQL - " + sql);

  cleanResultSet();
  std::vector<std::vector<std::string>> temp;
  int rc = sqlite3_exec(db, sql.c_str(), Repository::callback, (void*) &temp, &errMsg);

  PopulateResultSet(&temp);

  if (rc != SQLITE_OK) {
      log("SQL error: " + (std::string) errMsg); 
      sqlite3_free(errMsg);
  } else {
      log("Operation done successfully");
  }

  return rc == SQLITE_OK;
};