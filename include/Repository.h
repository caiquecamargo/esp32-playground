#include <Arduino.h>
#include "SPIFFS.h"
#include <sqlite3.h>
#include <vector>
#include <string>

bool replace(std::string& str, const std::string& from, const std::string& to);

template <class T>
class Repository {
  public:
    Repository(std::string fileName, std::string tableName);
    std::vector<T> resultSet;
    int isOpen();
    void close();
    void printResultSet();
  protected:
    sqlite3 *db;
    char *errMsg = 0;
    int opened;
    std::string fileName;
    std::string tableName;
    void open();
    int initialize();
    int createTableIfNotExists();
    void cleanResultSet();
    static int callback(void *data, int argc, char **argv, char **azColName);
    void PopulateResultSet(std::vector<std::vector<std::string>> *temp);
    int exec(std::string sql);
    int exists(std::string cardId);
};  