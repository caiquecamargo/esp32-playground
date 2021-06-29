#include "Repository.h" 
#include "model/User.h"

#ifndef USER_SERVICE_H
#define USER_SERVICE_H

class UserService : public Repository  {
  public: 
    std::vector<User> resultSet;
    UserService();
    void printResultSet();
    int create(User data);
    int update(User data);
    int findAll();
    int findById(std::string cardId);
    int findByName(std::string name);
    int deleteItem(std::string cardId);
  protected:
    int createTableIfNotExists();
    int exists(std::string cardId);
    void cleanResultSet();
    void PopulateResultSet(std::vector<std::vector<std::string>> *temp);
};

#endif