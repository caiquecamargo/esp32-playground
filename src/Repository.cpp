#include "Repository.h"

const static int CARD_ID_COLUMN = 0;
const static int NAME_COLUMN = 1;

const static String CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS %s (card_id PRIMARY KEY, name);";
const static String CREATE_SQL = "INSERT INTO %s VALUES(%s, %s)";
const static String UPDATE_SQL = "UPDATE %s SET name = %s WHERE card_id = %s";
const static String FIND_SQL = "SELECT * FROM %s";
const static String FIND_BY_ID_SQL = "SELECT * FROM %s WHERE card_id = %s";
const static String DELETE_SQL = "DELETE FROM %s WHERE card_id = %s";

int counter = 0;

int callback(void *data, int argc, char **argv, char **azColName) {
  std::vector<DB_DATA> *resultSet = (std::vector<DB_DATA> *) data;
  DB_DATA dbcontent;

  counter++;

  dbcontent.cardId = argv[CARD_ID_COLUMN];
  dbcontent.name = argv[NAME_COLUMN];

  Serial.printf("Chamou %d com %s e %s\n", counter, dbcontent.cardId.c_str(), dbcontent.name.c_str());

  resultSet->push_back(dbcontent);

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

Repository::Repository(const char *fileName, const char* tableName) {
  this->fileName = fileName;
  this->tableName = tableName;

  if (initialize()) {
    open();
    // deleteItem("");
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

void Repository::cleanResultSet() {
  resultSet.clear();
}

int Repository::exec(const char *sql) {
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
  // char *sql;
  // sprintf(sql, CREATE_TABLE_SQL.c_str(), tableName);
  return exec("CREATE TABLE IF NOT EXISTS test (card_id, name);");
};

int Repository::exists(std::string cardId) {
  std::string sql = "SELECT * FROM test WHERE card_id = ";
  exec(sql.append("'").append(cardId).append("'").c_str());

  Serial.printf("ResultSet size %d\n", resultSet.size());

  return resultSet.size();
}

int Repository::create(DB_DATA data) {
  // char *sql;
  // sprintf(sql, CREATE_SQL.c_str(), tableName, data.cardId.c_str(), data.name.c_str());

  return !exists("13454413245681") ? exec("INSERT INTO test VALUES('13454413245681', 'José da Silva')") : 0;
};

int Repository::update(String cardId, String name) {
  // char *sql;
  // sprintf(sql, UPDATE_SQL.c_str(), tableName, name.c_str(), cardId.c_str());

  // return exec(sql);
  return 0;
};

int Repository::findAll() {
  // char *sql;
  // sprintf(sql, FIND_SQL.c_str(), tableName);

  // return exec(sql);
  return exec("SELECT * FROM test");
};

int Repository::findById(String cardId) {
  // char *sql;
  // sprintf(sql, FIND_BY_ID_SQL.c_str(), tableName, cardId.c_str());

  // return exec(sql);
  return 0;
};

int Repository::deleteItem(String cardId) {
  // char *sql;
  // sprintf(sql, DELETE_SQL.c_str(), tableName, cardId.c_str());

  return exec("DROP TABLE IF EXISTS test");
  return 0;
};