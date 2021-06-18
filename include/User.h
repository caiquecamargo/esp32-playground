#include "Repository.h"

class User : Repository {
  public:
    User(std::string fileName) : Repository(fileName, "user_table");
};