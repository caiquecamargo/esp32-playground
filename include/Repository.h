#include <Arduino.h>
#include "SPIFFS.h"
#include <sqlite3.h>
#include <vector>

struct DB_DATA {
  String cardId;
  String name;
};

class Repository {
  public:
    Repository(const char *fileName);
    std::vector<DB_DATA> resultSet;
    int exec(const char *sql);
    int isOpen();
    void close();
    void printResultSet();
    int create(DB_DATA data);
    int update(String cardId, String name);
    int findAll();
    int findById(String cardId);
    int deleteItem(String cardId);
  private:
    sqlite3 *db;
    char *errMsg = 0;
    int opened;
    const char *fileName;
    const char *tableName;
    void open();
    int initialize();
    int createTableIfNotExists();
    void cleanResultSet();
};  