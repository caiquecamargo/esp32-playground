#include <Arduino.h>
#include "SPIFFS.h"
#include <sqlite3.h>
#include <vector>
#include <string>

struct DB_DATA {
  std::string cardId;
  std::string name;
};

class Repository {
  public:
    Repository(std::string fileName, std::string tableName);
    std::vector<DB_DATA> resultSet;
    int exec(std::string sql);
    int isOpen();
    void close();
    void printResultSet();
    int exists(std::string cardId);
    int create(DB_DATA data);
    int update(DB_DATA data);
    int findAll();
    int findById(std::string cardId);
    int deleteItem(std::string cardId);
  private:
    sqlite3 *db;
    char *errMsg = 0;
    int opened;
    std::string fileName;
    std::string tableName;
    void open();
    int initialize();
    int createTableIfNotExists();
    void cleanResultSet();
};  