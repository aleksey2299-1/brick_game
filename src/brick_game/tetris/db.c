#include <sqlite3.h>
#include <stdio.h>

int get_or_create_db(sqlite3 **database) {
  int result = 0;
  sqlite3 *db;
  if (sqlite3_open("score.db", &db) == SQLITE_OK) {
    *database = db;
  } else {
    sqlite3_close(db);
    result = 1;
  }
  return result;
}

void create_table(void) {
  sqlite3 *db;
  int result = get_or_create_db(&db);
  if (!result) {
    sqlite3_exec(db,
                 "CREATE TABLE IF NOT EXISTS tetris(id INTEGER PRIMARY KEY "
                 "AUTOINCREMENT, score INTEGER NOT NULL);",
                 0, NULL, NULL);
    sqlite3_close(db);
  }
}

int get_highest_score(void) {
  int highest_score = 0;
  sqlite3 *db;
  int result = get_or_create_db(&db);
  if (!result) {
    sqlite3_stmt *stmt;
    result =
        sqlite3_prepare_v2(db, "SELECT MAX(score) FROM tetris;", -1, &stmt, 0);
    if (result == SQLITE_OK) {
      result = sqlite3_step(stmt);
      if (result == SQLITE_ROW) {
        highest_score = sqlite3_column_int(stmt, 0);
      }
      sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
  }
  return highest_score;
}

void add_score(int score) {
  sqlite3 *db;
  int result = get_or_create_db(&db);
  if (!result) {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "INSERT INTO tetris(score) VALUES (?);", -1,
                           &stmt, 0) == SQLITE_OK) {
      if (sqlite3_bind_int(stmt, 1, score) == SQLITE_OK) {
        sqlite3_step(stmt);
      }
      sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
  }
}