#include "json.h"
#include "user.h"
#include <vector>

class UserSerializer : public JSON {
  public:
    std::vector<User> users;
    void create(User user);
    void create(std::vector<User> user);
    void add(User user);
    void add(std::vector<User> user);
    void erase();
    void createJson();
};