#include "User.h"

const static int CARD_ID_COLUMN = 0;
const static int NAME_COLUMN = 1;

const static String CREATE_SQL = "INSERT INTO %s VALUES(%s, %s)";
const static String UPDATE_SQL = "UPDATE %s SET name = %s WHERE card_id = %s";
const static String FIND_SQL = "SELECT * FROM %s";
const static String FIND_BY_ID_SQL = "SELECT * FROM %s WHERE card_id = %s";
const static String DELETE_SQL = "DELETE FROM %s WHERE card_id = %s";

User::User(const char *fileName) : Repository(fileName, "user_table") {}

void User::toObject(DB_USER *dbcontent, char **argv) {
  dbcontent->cardId = argv[CARD_ID_COLUMN];
  dbcontent->name = argv[NAME_COLUMN];
}

void User::printResultSet() {
  for (DB_USER content : resultSet) {
    Serial.printf("CARDID: %s, NAME: %s\n", content.cardId.c_str(), content.name.c_str());
  }

  Serial.println();
}

int User::create(DB_USER data) {
  char sql[CREATE_SQL.length()];
  sprintf(sql, CREATE_SQL.c_str(), tableName, data.cardId.c_str(), data.name.c_str());

  return exec(sql);
};

int User::update(String cardId, String name) {
  char sql[UPDATE_SQL.length()];
  sprintf(sql, UPDATE_SQL.c_str(), tableName, name.c_str(), cardId.c_str());

  return exec(sql);
};

int User::findAll() {
  char sql[FIND_SQL.length()];
  sprintf(sql, FIND_SQL.c_str(), tableName);

  return exec(sql);
};

int User::findById(String cardId) {
  char sql[FIND_BY_ID_SQL.length()];
  sprintf(sql, FIND_BY_ID_SQL.c_str(), tableName, cardId.c_str());

  return exec(sql);
};

int User::deleteItem(String cardId) {
  char sql[DELETE_SQL.length()];
  sprintf(sql, DELETE_SQL.c_str(), tableName, cardId.c_str());

  return exec(sql);
};