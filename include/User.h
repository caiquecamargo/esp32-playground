#include "Repository.h"

struct DB_USER {
  std::string cardId;
  std::string name;
};

class User : public Repository<DB_USER> {
  public:
    User(std::string fileName);
    int create(DB_USER data);
    int update(DB_USER data);
    int findAll();
    int findById(std::string cardId);
    int deleteItem(std::string cardId);
};