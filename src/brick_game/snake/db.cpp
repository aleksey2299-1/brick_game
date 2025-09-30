#include "./headers/db.h"

namespace brick_game {

Database::Database() {
  sqlite3_open("score.db", &db_);
  sqlite3_close(db_);
  db_ = nullptr;
}

Database::~Database() {
  if (db_ != nullptr) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

void Database::GetConnection() {
  if (sqlite3_open("score.db", &db_) != SQLITE_OK) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

void Database::CloseConnection() {
  if (db_) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

void Database::CreateTable(const std::string &table) {
  GetConnection();
  if (db_) {
    std::string stmt_string = "CREATE TABLE IF NOT EXISTS " + table +
                              "(id INTEGER PRIMARY KEY "
                              "AUTOINCREMENT, score INTEGER NOT NULL);";
    sqlite3_exec(db_, stmt_string.c_str(), 0, NULL, NULL);
    CloseConnection();
  }
}

int Database::GetHighestScore(const std::string &table) {
  int highest_score = 0;
  GetConnection();
  if (db_) {
    int result = false;
    std::string stmt_string = "SELECT MAX(score) FROM " + table + ";";
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(db_, stmt_string.c_str(), -1, &stmt, 0);
    if (result == SQLITE_OK) {
      result = sqlite3_step(stmt);
      if (result == SQLITE_ROW) {
        highest_score = sqlite3_column_int(stmt, 0);
      }
      sqlite3_finalize(stmt);
    }
    CloseConnection();
  }
  return highest_score;
}

void Database::InsertScore(int score, const std::string &table) {
  GetConnection();
  if (db_) {
    std::string stmt_string = "INSERT INTO " + table + "(score) VALUES (?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, stmt_string.c_str(), -1, &stmt, 0) ==
        SQLITE_OK) {
      if (sqlite3_bind_int(stmt, 1, score) == SQLITE_OK) {
        sqlite3_step(stmt);
      }
      sqlite3_finalize(stmt);
    }
    CloseConnection();
  }
}

}  // namespace brick_game
