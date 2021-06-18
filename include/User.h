#include "Repository.h"

struct DB_USER {
  std::string cardId;
  std::string name;
};

class User : public Repository {
  public:
    User(std::string fileName);
};