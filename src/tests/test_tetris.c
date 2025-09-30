#include <check.h>
#include <sqlite3.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>

#include "../brick_game/tetris/headers/db.h"
#include "../brick_game/tetris/headers/field.h"
#include "../brick_game/tetris/headers/figures.h"
#include "../brick_game/tetris/headers/game.h"
#include "../headers/game.h"

int delete_table() {
  int result = 1;
  sqlite3 *db;
  if (sqlite3_open("score.db", &db) == SQLITE_OK) {
    result = sqlite3_exec(db, "DROP TABLE IF EXISTS tetris;", 0, NULL, NULL);
  }
  sqlite3_close(db);
  return result;
}

START_TEST(test_create_field) {
  game_info_t game_info = {0};
  create_field(&game_info.field);

  ck_assert_ptr_ne(game_info.field, NULL);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game_info.field[i][j], 0);
    }
  }
  clear_field(&game_info.field);
}

START_TEST(test_change_orientation_0) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  rotate_figure(&figure, game_info.field);
  ck_assert_int_eq(figure.orientation, ROTATION_90);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_change_orientation_270) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.orientation = ROTATION_270;
  rotate_figure(&figure, game_info.field);
  ck_assert_int_eq(figure.orientation, ROTATION_0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_change_orientation_I_near_right_corner) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = I;
  figure.orientation = ROTATION_90;
  figure.position[1] = WIDTH - 1;
  rotate_figure(&figure, game_info.field);
  ck_assert_int_eq(figure.orientation, ROTATION_90);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_change_orientation_I_near_bottom) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = I;
  figure.orientation = ROTATION_0;
  figure.position[0] = HEIGHT - 2;
  rotate_figure(&figure, game_info.field);
  ck_assert_int_eq(figure.orientation, ROTATION_0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_change_orientation_I_near_busy_cell) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = I;
  figure.orientation = ROTATION_0;
  game_info.field[1][0] = 1;
  rotate_figure(&figure, game_info.field);
  ck_assert_int_eq(figure.orientation, ROTATION_0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_right) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 0;
  move_figure(&figure, RIGHT, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 1);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_right_near_corner) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = WIDTH - 2;
  move_figure(&figure, RIGHT, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], WIDTH - 2);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_right_near_busy_cell) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 0;
  game_info.field[0][2] = 1;
  move_figure(&figure, RIGHT, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_left) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 1;
  move_figure(&figure, LEFT, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_left_near_corner) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 0;
  move_figure(&figure, LEFT, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_left_near_busy_cell) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 1;
  game_info.field[0][0] = 1;
  move_figure(&figure, LEFT, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 1);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_down) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 0;
  move_figure(&figure, DOWN, game_info.field);
  ck_assert_int_eq(figure.position[0], 1);
  ck_assert_int_eq(figure.position[1], 0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_down_near_bottom) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = HEIGHT - 2;
  figure.position[1] = 0;
  move_figure(&figure, DOWN, game_info.field);
  ck_assert_int_eq(figure.position[0], HEIGHT - 2);
  ck_assert_int_eq(figure.position[1], 0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_move_O_down_near_busy_cell) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = UNFIXED;
  figure.position[0] = 0;
  figure.position[1] = 0;
  game_info.field[2][1] = 1;
  move_figure(&figure, DOWN, game_info.field);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 0);
  clear_field(&game_info.field);
}
END_TEST

START_TEST(test_spawn_I) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  int status = spawn_figure(&figure, I, game_info.field);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(figure.position[0], 0);
  ck_assert_int_eq(figure.position[1], 4);
  ck_assert_int_eq(figure.type, I);
  ck_assert_int_eq(figure.status, UNFIXED);
  clear_field(&game_info.field);
}

START_TEST(test_spawn_I_busy_cell) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  game_info.field[0][4] = 1;
  int status = spawn_figure(&figure, I, game_info.field);
  ck_assert_int_eq(status, 1);
  clear_field(&game_info.field);
}

START_TEST(test_attach) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = O;
  figure.status = FIXED;
  figure.position[0] = 3;
  figure.position[1] = 3;
  ck_assert_int_eq(game_info.field[3][3], 0);
  ck_assert_int_eq(game_info.field[3][4], 0);
  ck_assert_int_eq(game_info.field[4][3], 0);
  ck_assert_int_eq(game_info.field[4][4], 0);

  add_figure_to_field(&figure, game_info.field, 1);

  ck_assert_int_eq(game_info.field[3][3], 1);
  ck_assert_int_eq(game_info.field[3][4], 1);
  ck_assert_int_eq(game_info.field[4][3], 1);
  ck_assert_int_eq(game_info.field[4][4], 1);
  clear_field(&game_info.field);
}

START_TEST(test_attach_J_rotation_90) {
  game_info_t game_info = {0};
  create_field(&game_info.field);
  figure_t figure = {0};
  figure.type = J;
  figure.status = FIXED;
  figure.orientation = ROTATION_90;
  figure.position[0] = 3;
  figure.position[1] = 3;
  ck_assert_int_eq(game_info.field[3][4], 0);
  ck_assert_int_eq(game_info.field[4][4], 0);
  ck_assert_int_eq(game_info.field[4][4], 0);
  ck_assert_int_eq(game_info.field[5][4], 0);

  add_figure_to_field(&figure, game_info.field, 1);

  ck_assert_int_eq(game_info.field[3][4], 1);
  ck_assert_int_eq(game_info.field[4][4], 1);
  ck_assert_int_eq(game_info.field[4][4], 1);
  ck_assert_int_eq(game_info.field[5][4], 1);
  clear_field(&game_info.field);
}

START_TEST(test_delete_rows) {
  game_info_t game_info = {0};
  create_field(&game_info.field);

  game_info.field[10][4] = 1;
  game_info.field[15][4] = 1;

  for (int i = 0; i < WIDTH; i++) {
    game_info.field[13][i] = 1;
    game_info.field[14][i] = 1;
  }

  int cleared = check_rows(game_info.field);

  ck_assert_int_eq(cleared, 2);
  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(game_info.field[13][i], 0);
    ck_assert_int_eq(game_info.field[14][i], 0);
  }
  ck_assert_int_eq(game_info.field[10][4], 0);
  ck_assert_int_eq(game_info.field[12][4], 1);
  ck_assert_int_eq(game_info.field[15][4], 1);
  clear_field(&game_info.field);
}

START_TEST(test_get_info) {
  game_info_t *game_info = get_game_info();
  game_info->score = 500;
  game_info = NULL;
  game_info = get_game_info();

  ck_assert_int_eq(game_info->score, 500);
}

START_TEST(test_UserInput_start) {
  game_info_t *game_info = get_game_info();
  ck_assert_ptr_eq(game_info->field, NULL);

  UserInput(Start, false);

  ck_assert_ptr_ne(game_info->field, NULL);
}

START_TEST(test_UserInput_down) {
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  spawn_figure(&game_info->figure, O, game_info->field);
  game_info->state = MOVING;

  ck_assert_int_eq(game_info->figure.position[0], 0);

  UserInput(Down, false);

  ck_assert_int_eq(game_info->figure.position[0], 1);
  clear_field(&game_info->field);
}

START_TEST(test_UserInput_left) {
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  spawn_figure(&game_info->figure, O, game_info->field);
  game_info->state = MOVING;

  ck_assert_int_eq(game_info->figure.position[1], 4);

  UserInput(Left, false);

  ck_assert_int_eq(game_info->figure.position[1], 3);
  clear_field(&game_info->field);
}

START_TEST(test_UserInput_right) {
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  spawn_figure(&game_info->figure, O, game_info->field);
  game_info->state = MOVING;

  ck_assert_int_eq(game_info->figure.position[1], 4);

  UserInput(Right, false);

  ck_assert_int_eq(game_info->figure.position[1], 5);
  clear_field(&game_info->field);
}

START_TEST(test_UserInput) {
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  spawn_figure(&game_info->figure, O, game_info->field);
  game_info->state = MOVING;

  ck_assert_int_eq(game_info->figure.orientation, ROTATION_0);

  UserInput(Action, false);

  ck_assert_int_eq(game_info->figure.orientation, ROTATION_90);
  clear_field(&game_info->field);
}

START_TEST(test_UserInput_pause) {
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  spawn_figure(&game_info->figure, O, game_info->field);
  game_info->state = MOVING;

  ck_assert_int_eq(game_info->figure.position[1], 4);

  UserInput(Pause, false);
  UserInput(Right, false);

  ck_assert_int_eq(game_info->figure.position[1], 4);
  clear_field(&game_info->field);
}

START_TEST(test_game_loop_3ticks) {
  struct timespec ts = {0};
  ts.tv_nsec = 500 * 1000;
  game_info_t *game_info = get_game_info();
  game_info->state = STARTING;
  create_field(&game_info->field);

  UpdateCurrentState();
  game_info->speed = 500;
  UpdateCurrentState();

  for (int i = 0; i < 4; i++) {
    UpdateCurrentState();
    thrd_sleep(&ts, NULL);
  }

  ck_assert_int_eq(game_info->state, MOVING);
  ck_assert_int_eq(game_info->figure.position[0], 3);
  clear_field(&game_info->field);
}

START_TEST(test_game_loop_attaching_Z) {
  struct timespec ts = {0};
  ts.tv_nsec = 500 * 1000;
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  game_info->next_figure = Z;
  game_info->state = SPAWNING;
  game_info->speed = 500;
  game_info->level = 1;

  for (int i = 0; i < WIDTH; i++) {
    game_info->field[3][i] = 1;
    game_info->field[4][i] = 1;
  }

  game_info->field[3][5] = 0;
  game_info->field[3][6] = 0;

  UpdateCurrentState();

  while (game_info->state == MOVING) {
    UpdateCurrentState();
    thrd_sleep(&ts, NULL);
  }

  UpdateCurrentState();

  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(game_info->field[3][i], 0);
    if (i != 4 && i != 5) {
      ck_assert_int_eq(game_info->field[4][i], 0);
    }
  }
  ck_assert_int_eq(game_info->field[4][4], 1);
  ck_assert_int_eq(game_info->field[4][5], 1);
  ck_assert_int_eq(game_info->score, 300);
  ck_assert_int_eq(game_info->state, SPAWNING);
  clear_field(&game_info->field);
}

START_TEST(test_game_loop_attaching_I_bottom) {
  struct timespec ts = {0};
  ts.tv_nsec = 500 * 1000;
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  game_info->next_figure = I;
  game_info->state = SPAWNING;
  game_info->speed = 500;
  game_info->level = 1;

  for (int i = 0; i < WIDTH; i++) {
    game_info->field[16][i] = 1;
    game_info->field[17][i] = 1;
    game_info->field[18][i] = 1;
    game_info->field[19][i] = 1;
  }

  game_info->field[16][4] = 0;
  game_info->field[17][4] = 0;
  game_info->field[18][4] = 0;
  game_info->field[19][4] = 0;

  UpdateCurrentState();
  game_info->figure.orientation = ROTATION_90;

  while (game_info->state == MOVING) {
    UpdateCurrentState();
    thrd_sleep(&ts, NULL);
  }

  UpdateCurrentState();

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game_info->field[i][j], 0);
    }
  }
  ck_assert_int_eq(game_info->score, 1500);
  ck_assert_int_eq(game_info->level, 3);
  ck_assert_int_eq(game_info->state, SPAWNING);
  clear_field(&game_info->field);
}

START_TEST(test_game_loop_gameover) {
  game_info_t *game_info = get_game_info();
  create_field(&game_info->field);
  game_info->state = SPAWNING;

  game_info->field[0][4] = 1;

  UpdateCurrentState();

  ck_assert_int_eq(game_info->state, GAMEOVER);

  UpdateCurrentState();

  ck_assert_int_eq(game_info->state, WAITING);

  clear_field(&game_info->field);
}

START_TEST(test_db_table) {
  int failed = delete_table();
  if (!failed) {
    sqlite3 *db;
    if (sqlite3_open("score.db", &db) == SQLITE_OK) {
      sqlite3_stmt *stmt;
      const char *sql =
          "SELECT name FROM sqlite_master WHERE type='table' AND "
          "name='tetris';";
      if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        int result = sqlite3_step(stmt);
        ck_assert_int_eq(result, SQLITE_DONE);
        create_table();
        result = sqlite3_step(stmt);
        ck_assert_int_eq(result, SQLITE_ROW);
        sqlite3_finalize(stmt);
      }
    }
    sqlite3_close(db);
  }
}

START_TEST(test_db_table_highest_score) {
  int failed = delete_table();
  if (!failed) {
    sqlite3 *db;
    if (sqlite3_open("score.db", &db) == SQLITE_OK) {
      create_table();
      const char *sql = "INSERT INTO tetris(score) VALUES (300), (1700), (0);";
      if (sqlite3_exec(db, sql, 0, NULL, NULL) == SQLITE_OK) {
        int result = get_highest_score();
        ck_assert_int_eq(result, 1700);
      }
    }
    sqlite3_close(db);
  }
}

START_TEST(test_db_table_highest_score_after_gameover) {
  int failed = delete_table();
  if (!failed) {
    sqlite3 *db;
    if (sqlite3_open("score.db", &db) == SQLITE_OK) {
      game_info_t *game_info = get_game_info();
      game_info->state = SPAWNING;
      game_info->score = 6000;
      create_field(&game_info->field);
      create_table();

      UserInput(Terminate, false);

      ck_assert_int_eq(game_info->state, GAMEOVER);

      UpdateCurrentState();

      int result = get_highest_score();
      ck_assert_int_eq(result, 6000);
      ck_assert_int_eq(game_info->state, WAITING);

      clear_field(&game_info->field);
    }
    sqlite3_close(db);
  }
}

START_TEST(test_clear_field_after_game) {
  game_info_t *game_info = get_game_info();
  GameInfo_t *gi = get_game_info_for_front();
  game_info->state = SPAWNING;
  game_info->score = 6000;
  create_field(&game_info->field);
  create_table();

  UserInput(Terminate, false);

  ck_assert_int_eq(game_info->state, GAMEOVER);

  UpdateCurrentState();

  ck_assert_int_eq(game_info->state, WAITING);
  ck_assert_ptr_ne(game_info->field, NULL);

  UserInput(Terminate, false);

  ck_assert_ptr_eq(game_info->field, NULL);
  ck_assert_ptr_eq(gi->field, NULL);
  ck_assert_ptr_eq(gi->next, NULL);
}

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tetris;

  s = suite_create("tetris");

  tetris = tcase_create("tetris");

  tcase_add_test(tetris, test_create_field);
  tcase_add_test(tetris, test_change_orientation_0);
  tcase_add_test(tetris, test_change_orientation_270);
  tcase_add_test(tetris, test_change_orientation_I_near_right_corner);
  tcase_add_test(tetris, test_change_orientation_I_near_bottom);
  tcase_add_test(tetris, test_change_orientation_I_near_busy_cell);
  tcase_add_test(tetris, test_move_O_right);
  tcase_add_test(tetris, test_move_O_right_near_corner);
  tcase_add_test(tetris, test_move_O_right_near_busy_cell);
  tcase_add_test(tetris, test_move_O_left);
  tcase_add_test(tetris, test_move_O_left_near_corner);
  tcase_add_test(tetris, test_move_O_left_near_busy_cell);
  tcase_add_test(tetris, test_move_O_down);
  tcase_add_test(tetris, test_move_O_down_near_bottom);
  tcase_add_test(tetris, test_move_O_down_near_busy_cell);
  tcase_add_test(tetris, test_spawn_I);
  tcase_add_test(tetris, test_spawn_I_busy_cell);
  tcase_add_test(tetris, test_attach);
  tcase_add_test(tetris, test_attach_J_rotation_90);
  tcase_add_test(tetris, test_delete_rows);
  tcase_add_test(tetris, test_get_info);
  tcase_add_test(tetris, test_UserInput_start);
  tcase_add_test(tetris, test_UserInput_down);
  tcase_add_test(tetris, test_UserInput_right);
  tcase_add_test(tetris, test_UserInput_left);
  tcase_add_test(tetris, test_UserInput);
  tcase_add_test(tetris, test_UserInput_pause);
  tcase_add_test(tetris, test_game_loop_3ticks);
  tcase_add_test(tetris, test_game_loop_attaching_Z);
  tcase_add_test(tetris, test_game_loop_attaching_I_bottom);
  tcase_add_test(tetris, test_game_loop_gameover);
  tcase_add_test(tetris, test_db_table);
  tcase_add_test(tetris, test_db_table_highest_score);
  tcase_add_test(tetris, test_db_table_highest_score_after_gameover);
  tcase_add_test(tetris, test_clear_field_after_game);

  suite_add_tcase(s, tetris);

  return s;
}

int main() {
  int number_failed = 0;
  Suite *su;
  SRunner *sr;

  su = tetris_suite();
  sr = srunner_create(su);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}