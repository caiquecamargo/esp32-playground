#include <Arduino.h>
#include "SPIFFS.h"
#include <sqlite3.h>
#include <vector>
#include <string>

struct DB_DATA {
  std::string cardId;
  std::string name;
};

bool replace(std::string& str, const std::string& from, const std::string& to);

class Repository {
  public:
    Repository(std::string fileName, std::string tableName);
    int isOpen();
    void close();
    virtual void printResultSet() = 0;
  protected:
    sqlite3 *db;
    char *errMsg = 0;
    int opened;
    std::string fileName;
    std::string tableName;
    void init();
    void open();
    int initialize();
    virtual int createTableIfNotExists() = 0;
    virtual void cleanResultSet() = 0;
    static int callback(void *data, int argc, char **argv, char **azColName);
    virtual void PopulateResultSet(std::vector<std::vector<std::string>> *temp) = 0;
    int exec(std::string sql);
    virtual int exists(std::string cardId) = 0;
};  