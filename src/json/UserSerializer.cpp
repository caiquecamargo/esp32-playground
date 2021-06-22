#include "json/UserSerializer.h"

std::string UserSerializer::createJson (User user) {
  clear();

  jsonDocument["cardId"] = user.cardId;
  jsonDocument["name"] = user.name;

  return stringify();
}

std::string UserSerializer::createJson (std::vector<User> users) {
  clear();

  for (int i = 0; i < users.size(); i++) {
    jsonDocument["users"][i]["cardId"] = users[i].cardId;
    jsonDocument["users"][i]["name"] = users[i].name;
  }

  return stringify();
}