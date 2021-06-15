#include <Arduino.h>
#include "SPIFFS.h"
#include <sqlite3.h>
#include <vector>

template <typename T>
class Repository {
  public:
    Repository(const char *fileName, const char *tableName);
    std::vector<T> resultSet;
    int exec(const char *sql);
    int isOpen();
    void close();
    virtual void printResultSet() = 0;
    virtual int create(T data) = 0;
    virtual int update(String cardId, String name) = 0;
    virtual int findAll() = 0;
    virtual int findById(String cardId) = 0;
    virtual int deleteItem(String cardId) = 0;
  protected:
    sqlite3 *db;
    char *errMsg = 0;
    int opened;
    const char *fileName;
    const char *tableName;
    int callback(void *data, int argc, char **argv, char **azColName);
    virtual void toObject(T *dbcontent, char **argv) = 0;
    void open();
    int initialize();
    int createTableIfNotExists();
    void cleanResultSet();
};  