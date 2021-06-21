#include "Json.h"
#include "model/User.h"
#include <vector>

class UserSerializer : public JSON {
  public:
    std::string createJson(User user);
    std::string createJson(std::vector<User> users);
};