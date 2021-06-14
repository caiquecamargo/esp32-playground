#include <Arduino.h>
#include "SPIFFS.h"
#include <sqlite3.h>
#include <vector>

struct DBCONTENT {
  int id;
  String name;
  String cardId;
};

class Repository {
  public:
    Repository(const char *fileName);
    int exec(const char *sql);
    int isOpen();
    void close();
    void print();
  private:
    sqlite3 *db;
    char *errMsg = 0;
    int opened;
    std::vector<DBCONTENT> dbcontentList;
    const char *fileName;
    void open();
    int initialize();
};  