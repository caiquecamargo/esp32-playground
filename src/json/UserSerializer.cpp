#include "json/UserSerializer.h"

void UserSerializer::createJson () {
  for (User& user :users) {
    jsonDocument["cardId"] = user.cardId;
    jsonDocument["name"] = user.name;
  }
}

void UserSerializer::erase() {
  users.clear();
}

void UserSerializer::create(User user) {
  erase();

  users.push_back(user);
}

void UserSerializer::create(std::vector<User> usersList) {
  erase();

  for (User& user : usersList) {
    users.push_back(user);
  }
}

void UserSerializer::add(User user) {
  users.push_back(user);
}

void UserSerializer::add(std::vector<User> usersList) {
  for (User& user : usersList) {
    users.push_back(user);
  }
}