#include "User.h"

const static std::string CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS {0} (card_id, name);";
const static std::string EXISTS_SQL = "SELECT * FROM {0} WHERE card_id = '{1}'";
const static std::string CREATE_SQL = "INSERT INTO {0} VALUES('{1}', '{2}')";
const static std::string UPDATE_SQL = "UPDATE {0} SET name = '{1}' WHERE card_id = '{2}'";
const static std::string FIND_SQL = "SELECT * FROM {0}";
const static std::string FIND_BY_ID_SQL = "SELECT * FROM {0} WHERE card_id = '{1}'";
const static std::string DELETE_SQL = "DELETE FROM {0} WHERE card_id = '{1}'";

User::User(std::string fileName) : Repository<DB_USER>(fileName, "user_table") {};

int User::create(DB_USER data) {
  if (exists(data.cardId)) return 0;

  std::string sql = CREATE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", data.cardId);
  replace(sql, "{2}", data.name);

  return this->exec(sql);
};

int User::update(DB_USER data) {
  if (!exists(data.cardId)) return 0;

  std::string sql = UPDATE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", data.name);
  replace(sql, "{2}", data.cardId);

  return this->exec(sql);
};

int User::findAll() {
  std::string sql = FIND_SQL;
  replace(sql, "{0}", tableName);

  return this->exec(sql);
};

int User::findById(std::string cardId) {
  std::string sql = FIND_BY_ID_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  return this->exec(sql);
};

int User::deleteItem(std::string cardId) {
  if (!exists(cardId)) return 0;

  std::string sql = DELETE_SQL;
  replace(sql, "{0}", tableName);
  replace(sql, "{1}", cardId);

  return this->exec(sql);
};