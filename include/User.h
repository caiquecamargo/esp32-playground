#include "Repository.h"

struct DB_USER {
  String cardId;
  String name;
};

class User : public Repository<DB_USER> {
  public:
    User(const char *fileName);
    void printResultSet();
    int create(DB_USER data);
    int update(String cardId, String name);
    int findAll();
    int findById(String cardId);
    int deleteItem(String cardId);
  protected:
    void toObject(DB_USER *dbcontent, char **argv);
};