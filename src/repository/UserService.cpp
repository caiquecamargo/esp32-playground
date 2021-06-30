#include "repository/UserService.h"

const static std::string CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS {0} (card_id, name);";
const static std::string EXISTS_SQL = "SELECT * FROM {0} WHERE card_id = '{1}'";
const static std::string CREATE_SQL = "INSERT INTO {0} VALUES('{1}', '{2}')";
const static std::string UPDATE_SQL = "UPDATE {0} SET name = '{1}' WHERE card_id = '{2}'";
const static std::string FIND_SQL = "SELECT * FROM {0}";
const static std::string FIND_BY_ID_SQL = "SELECT * FROM {0} WHERE card_id = '{1}'";
const static std::string FIND_BY_NAME_SQL = "SELECT * FROM {0} WHERE name LIKE '%{1}%'";
const static std::string DELETE_SQL = "DELETE FROM {0} WHERE card_id = '{1}'";

const static char* FILE_NAME = "/spiffs/user.db";
const static char* TABLE_NAME = "user_table";

UserService::UserService() : Repository(FILE_NAME, TABLE_NAME) { };

void UserService::cleanResultSet() {
  resultSet.clear();
}

void UserService::PopulateResultSet(std::vector<std::vector<std::string>> *temp) {
  if (temp->size() == 0) return;

  while (!temp->empty()) {
    std::vector<std::string> dataTemp = temp->back();
    User user;
    user.name = dataTemp.back();
    dataTemp.pop_back();
    user.cardId = dataTemp.back();
    dataTemp.pop_back();

    resultSet.push_back(user);
    temp->pop_back();
  }
};

void UserService::printResultSet() {
  if (resultSet.size() == 0) return;

  Serial.println("DB Search Result.");
  for (User content : resultSet) {
    Serial.printf("CARDID: %s, NAME: %s\n", content.cardId.c_str(), content.name.c_str());
  }

  Serial.println();
}

int UserService::createTableIfNotExists() {
  std::string sql = CREATE_TABLE_SQL;
  replace(sql, "{0}", tableName);

  return exec(sql);
};

int UserService::exists(std::string cardId) {
  std::string sql = EXISTS_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  exec(sql);

  return resultSet.size();
}

int UserService::create(User data) {
  if (exists(data.cardId)) return 0;

  std::string sql = CREATE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", data.cardId);
  replace(sql, "{2}", data.name);

  return exec(sql);
};


int UserService::update(User data) {
  if (!exists(data.cardId)) return 0;

  std::string sql = UPDATE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", data.name);
  replace(sql, "{2}", data.cardId);

  return exec(sql);
};


int UserService::findAll() {
  std::string sql = FIND_SQL;
  replace(sql, "{0}", tableName);

  return exec(sql);
};


int UserService::findById(std::string cardId) {
  std::string sql = FIND_BY_ID_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  Serial.println(sql.c_str());

  return exec(sql);
};

int UserService::findByName(std::string name) {
  std::string sql = FIND_BY_NAME_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", name);

  return exec(sql);
};


int UserService::deleteItem(std::string cardId) {
  if (!exists(cardId)) return 0;

  std::string sql = DELETE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  return exec(sql);
};