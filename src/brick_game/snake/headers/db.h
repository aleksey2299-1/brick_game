#ifndef BRICK_GAME_SNAKE_DB_H
#define BRICK_GAME_SNAKE_DB_H

#include <sqlite3.h>

#include <string>

namespace brick_game {

class Database {
 public:
  Database();
  ~Database();

  int GetHighestScore(const std::string &table);
  void InsertScore(int score, const std::string &table);
  void CreateTable(const std::string &table);

 private:
  sqlite3 *db_;

  void GetConnection();
  void CloseConnection();
};

}  // namespace brick_game

#endif