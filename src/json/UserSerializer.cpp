#include "json/UserSerializer.h"

std::string UserSerializer::createJson (User user) {
  clear();

  jsonDocument["cardId"] = user.cardId;
  jsonDocument["name"] = user.name;

  return stringify();
}

std::string UserSerializer::createJson (std::vector<User> users) {
  clear();

  for (User& user :users) {
    jsonDocument["cardId"] = user.cardId;
    jsonDocument["name"] = user.name;
  }

  return stringify();
}