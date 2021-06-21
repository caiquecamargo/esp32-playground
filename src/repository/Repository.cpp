#include "repository/Repository.h"

const static int CARD_ID_COLUMN = 0;
const static int NAME_COLUMN = 1;

const static std::string CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS {0} (card_id, name);";
const static std::string EXISTS_SQL = "SELECT * FROM {0} WHERE card_id = '{1}'";
const static std::string CREATE_SQL = "INSERT INTO {0} VALUES('{1}', '{2}')";
const static std::string UPDATE_SQL = "UPDATE {0} SET name = '{1}' WHERE card_id = '{2}'";
const static std::string FIND_SQL = "SELECT * FROM {0}";
const static std::string FIND_BY_ID_SQL = "SELECT * FROM {0} WHERE card_id = '{1}'";
const static std::string DELETE_SQL = "DELETE FROM {0} WHERE card_id = '{1}'";

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

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

Repository::Repository(std::string fileName, std::string tableName) {
  this->fileName = fileName;
  this->tableName = tableName;

  if (initialize()) {
    open();
    createTableIfNotExists();
  }
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

void Repository::cleanResultSet() {
  resultSet.clear();
}

void Repository::PopulateResultSet(std::vector<std::vector<std::string>> *temp) {
  if (temp->size() == 0) return;

  while (!temp->empty()) {
    std::vector<std::string> dataTemp = temp->back();
    DB_DATA dbdata;
    dbdata.name = dataTemp.back();
    dataTemp.pop_back();
    dbdata.cardId = dataTemp.back();
    dataTemp.pop_back();

    resultSet.push_back(dbdata);
    temp->pop_back();
  }
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
  int rc = sqlite3_exec(db, sql.c_str(), callback, (void*) &temp, &errMsg);

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
  if (resultSet.size() != 0) printResultSet();

  return rc;
};

void Repository::printResultSet() {
  Serial.println("DB Search Result.");
  for (DB_DATA content : resultSet) {
    Serial.printf("CARDID: %s, NAME: %s\n", content.cardId.c_str(), content.name.c_str());
  }

  Serial.println();
}

int Repository::createTableIfNotExists() {
  std::string sql = CREATE_TABLE_SQL;
  replace(sql, "{0}", tableName);

  return exec(sql);
};

int Repository::exists(std::string cardId) {
  std::string sql = EXISTS_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  exec(sql);

  return resultSet.size();
}

int Repository::create(DB_DATA data) {
  if (exists(data.cardId)) return 0;

  std::string sql = CREATE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", data.cardId);
  replace(sql, "{2}", data.name);

  return exec(sql);
};


int Repository::update(DB_DATA data) {
  if (!exists(data.cardId)) return 0;

  std::string sql = UPDATE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", data.name);
  replace(sql, "{2}", data.cardId);

  return exec(sql);
};


int Repository::findAll() {
  std::string sql = FIND_SQL;
  replace(sql, "{0}", tableName);

  return exec(sql);
};


int Repository::findById(std::string cardId) {
  std::string sql = FIND_BY_ID_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  return exec(sql);
};


int Repository::deleteItem(std::string cardId) {
  if (!exists(cardId)) return 0;

  std::string sql = DELETE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  return exec(sql);
};