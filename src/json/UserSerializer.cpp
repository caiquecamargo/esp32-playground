#include "json/UserSerializer.h"

std::string UserSerializer::error (std::string error) {
  clear();

  jsonDocument["error"] = error;

  return stringify();
}

std::string UserSerializer::createJson (User user) {
  clear();

  jsonDocument["data"]["cardId"] = user.cardId;
  jsonDocument["data"]["name"] = user.name;

  return stringify();
}

std::string UserSerializer::createJson (std::vector<User> users) {
  clear();

  if (users.size() == 0) jsonDocument["data"][0] = NULL;

  for (int i = 0; i < users.size(); i++) {
    jsonDocument["data"][i]["cardId"] = users[i].cardId;
    jsonDocument["data"][i]["name"] = users[i].name;
  }

  return stringify();
}