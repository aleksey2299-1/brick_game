#include <gtest/gtest.h>

#include "../brick_game/snake/headers/controller.h"
#include "../brick_game/snake/headers/db.h"
#include "../brick_game/snake/headers/model.h"
#include "../brick_game/snake/headers/snake.h"

int **CreateField() {
  int **field = new int *[HEIGHT];
  for (auto i = 0; i < HEIGHT; i++) {
    field[i] = new int[WIDTH]();
  }
  return field;
}

void ClearField(int **field) {
  if (field) {
    for (auto i = 0; i < HEIGHT; i++) {
      delete[] field[i];
    }
    delete[] field;
    field = nullptr;
  }
}

namespace brick_game {

TEST(Snake, Create) {
  Snake snake;
  int **field = CreateField();

  snake.GetBody(field);

  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);
  EXPECT_EQ(field[9][3], 1);
  EXPECT_EQ(snake.GetLength(), 4);

  ClearField(field);
}

TEST(Snake, Copy) {
  Snake snake;
  int **field = CreateField();

  snake.GetBody(field);

  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);
  EXPECT_EQ(field[9][3], 1);
  EXPECT_EQ(snake.GetLength(), 4);

  Snake snake1(snake);

  ClearField(field);
  field = CreateField();

  snake1.GetBody(field);

  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);
  EXPECT_EQ(field[9][3], 1);
  EXPECT_EQ(snake1.GetLength(), 4);

  snake1.Clear();

  EXPECT_EQ(snake1.GetLength(), 0);
  EXPECT_EQ(snake.GetLength(), 4);

  ClearField(field);
}

TEST(Snake, MoveAssignment) {
  Snake snake;
  Snake snake1;

  EXPECT_EQ(snake.GetLength(), 4);
  EXPECT_EQ(snake1.GetLength(), 4);
  snake.Clear();

  snake1 = std::move(snake);

  EXPECT_EQ(snake1.GetLength(), 0);
}

TEST(Snake, Clear) {
  Snake snake;

  snake.Clear();

  EXPECT_EQ(snake.GetLength(), 0);
}

TEST(Snake, Spawn) {
  Snake snake;
  int **field = CreateField();

  snake.Clear();

  EXPECT_EQ(snake.GetLength(), 0);

  snake.Spawn();
  snake.GetBody(field);

  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);
  EXPECT_EQ(field[9][3], 1);
  EXPECT_EQ(snake.GetLength(), 4);

  ClearField(field);
}

TEST(Snake, Move) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[9][7], 1);
  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);
  EXPECT_EQ(field[9][3], 0);

  ClearField(field);
}

TEST(Snake, MoveEat) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {9, 7};

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[9][7], 1);
  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);
  EXPECT_EQ(field[9][3], 1);

  ClearField(field);
}

TEST(Snake, MoveBorderRight) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};
  bool result = false;

  for (auto i = 4; i > 0; i--) {
    result = snake.Move(eat);
  }

  EXPECT_EQ(result, true);

  snake.GetBody(field);

  EXPECT_EQ(field[9][9], 1);
  EXPECT_EQ(field[9][8], 1);
  EXPECT_EQ(field[9][7], 1);
  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 0);

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[9][9], 1);
  EXPECT_EQ(field[9][8], 1);
  EXPECT_EQ(field[9][7], 1);
  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 0);

  ClearField(field);
}

TEST(Snake, MoveBorderTop) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};
  bool result = false;

  snake.RotateSnake(Left);

  for (auto i = 10; i > 0; i--) {
    result = snake.Move(eat);
  }

  EXPECT_EQ(result, true);

  snake.GetBody(field);

  EXPECT_EQ(field[0][6], 1);
  EXPECT_EQ(field[1][6], 1);
  EXPECT_EQ(field[2][6], 1);
  EXPECT_EQ(field[3][6], 1);
  EXPECT_EQ(field[4][6], 0);

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[0][6], 1);
  EXPECT_EQ(field[1][6], 1);
  EXPECT_EQ(field[2][6], 1);
  EXPECT_EQ(field[3][6], 1);
  EXPECT_EQ(field[4][6], 0);

  ClearField(field);
}

TEST(Snake, MoveBorderBottom) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};
  bool result = false;

  snake.RotateSnake(Right);

  for (auto i = 11; i > 0; i--) {
    result = snake.Move(eat);
  }

  EXPECT_EQ(result, true);

  snake.GetBody(field);

  EXPECT_EQ(field[19][6], 1);
  EXPECT_EQ(field[18][6], 1);
  EXPECT_EQ(field[17][6], 1);
  EXPECT_EQ(field[16][6], 1);
  EXPECT_EQ(field[15][6], 0);

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[19][6], 1);
  EXPECT_EQ(field[18][6], 1);
  EXPECT_EQ(field[17][6], 1);
  EXPECT_EQ(field[16][6], 1);
  EXPECT_EQ(field[15][6], 0);

  ClearField(field);
}

TEST(Snake, MoveBorderLeft) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};
  bool result = false;

  snake.RotateSnake(Right);
  snake.Move(eat);
  snake.RotateSnake(Right);

  for (auto i = 7; i > 0; i--) {
    result = snake.Move(eat);
  }

  EXPECT_EQ(result, true);

  snake.GetBody(field);

  EXPECT_EQ(field[10][0], 1);
  EXPECT_EQ(field[10][1], 1);
  EXPECT_EQ(field[10][2], 1);
  EXPECT_EQ(field[10][3], 1);
  EXPECT_EQ(field[10][4], 0);

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[10][0], 1);
  EXPECT_EQ(field[10][1], 1);
  EXPECT_EQ(field[10][2], 1);
  EXPECT_EQ(field[10][3], 1);
  EXPECT_EQ(field[10][4], 0);

  ClearField(field);
}

TEST(Snake, CantEatSelfWithFourPieces) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};
  bool result = false;

  snake.RotateSnake(Right);
  snake.Move(eat);
  snake.RotateSnake(Right);
  snake.Move(eat);
  snake.RotateSnake(Right);
  result = snake.Move(eat);

  EXPECT_EQ(result, false);

  snake.GetBody(field);

  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[10][5], 1);
  EXPECT_EQ(field[10][6], 1);
  EXPECT_EQ(field[9][6], 1);

  result = snake.Move(eat);

  EXPECT_EQ(result, false);

  snake.GetBody(field);

  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[10][5], 1);
  EXPECT_EQ(field[10][6], 1);
  EXPECT_EQ(field[9][6], 1);

  ClearField(field);
}

TEST(Snake, EatSelf) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {10, 6};
  bool result = false;

  snake.RotateSnake(Right);
  snake.Move(eat);

  eat[0] = 0;
  eat[1] = 0;

  snake.RotateSnake(Right);
  snake.Move(eat);
  snake.RotateSnake(Right);
  result = snake.Move(eat);

  EXPECT_EQ(result, true);

  snake.GetBody(field);

  EXPECT_EQ(field[10][5], 1);
  EXPECT_EQ(field[10][6], 1);
  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 0);
  EXPECT_EQ(snake.GetLength(), 5);

  ClearField(field);
}

TEST(Snake, MoveRotate) {
  Snake snake;
  int **field = CreateField();
  int eat[2] = {0};

  snake.RotateSnake(Left);

  snake.Move(eat);
  snake.GetBody(field);

  EXPECT_EQ(field[8][6], 1);
  EXPECT_EQ(field[9][6], 1);
  EXPECT_EQ(field[9][5], 1);
  EXPECT_EQ(field[9][4], 1);

  ClearField(field);
}

TEST(Model, Create) {
  Model model;

  GameInfo_t info = model.GetCurrentState();

  EXPECT_EQ(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.high_score, 0);
  EXPECT_EQ(info.pause, false);
}

TEST(Model, Copy) {
  Model model;

  model.Start();
  model.UpdateState();
  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  Model model2(model);
  model.UpdateState();

  info = model2.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  model2.Clear();
  info = model2.GetCurrentState();

  EXPECT_EQ(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.pause, false);

  info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);
}

TEST(Model, MoveAssignment) {
  Model model;
  Model model1;

  model.Start();
  model.UpdateState();
  model.PauseGame();
  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, true);

  info = model1.GetCurrentState();

  EXPECT_EQ(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.pause, false);

  model1 = std::move(model);

  info = model1.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, true);
}

TEST(Model, Clear) {
  Model model;

  model.Start();
  model.Clear();
  GameInfo_t info = model.GetCurrentState();

  EXPECT_EQ(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.high_score, 0);
  EXPECT_EQ(info.pause, false);
}

TEST(Model, GameOver) {
  Model model;

  model.GameOver();
  GameInfo_t info = model.GetCurrentState();

  EXPECT_EQ(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.high_score, 0);
  EXPECT_EQ(info.pause, false);
}

TEST(Model, Start) {
  Model model;

  model.Start();
  model.UpdateState();

  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);
  EXPECT_NE(info.field[9][3], 0);

  EXPECT_NE(info.field[5][5], 0);
}

TEST(Model, PauseGame) {
  Model model;

  model.Start();
  model.UpdateState();
  model.PauseGame();

  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, true);

  model.MoveSnake();
  info = model.GetCurrentState();

  EXPECT_EQ(info.pause, true);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);
  EXPECT_NE(info.field[9][3], 0);

  EXPECT_NE(info.field[5][5], 0);

  model.PauseGame();

  info = model.GetCurrentState();

  EXPECT_EQ(info.pause, false);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);
  EXPECT_NE(info.field[9][3], 0);

  EXPECT_NE(info.field[5][5], 0);
}

TEST(Model, MoveLeft) {
  Model model;

  model.Start();
  model.UpdateState();
  model.RefreshTimer();
  model.MoveLeft();
  model.UpdateState();

  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[8][6], 0);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);

  EXPECT_NE(info.field[5][5], 0);
}

TEST(Model, MoveRight) {
  Model model;

  model.Start();
  model.UpdateState();
  model.RefreshTimer();
  model.MoveRight();
  model.UpdateState();

  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[10][6], 0);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);

  EXPECT_NE(info.field[5][5], 0);
}

TEST(Model, Victory) {
  Model model;

  model.Start();
  model.UpdateState();

  for (auto i = 0; i < 199; i++) {
    model.AddScore();
  }

  GameInfo_t info = model.GetCurrentState();

  EXPECT_EQ(info.level, 10);
  EXPECT_EQ(info.score, 199);

  model.AddScore();
  info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 200);
  EXPECT_EQ(info.speed, SPEED / 1000 / 10);
  EXPECT_EQ(info.level, VICTORY);
  EXPECT_EQ(info.pause, false);
}

TEST(Model, Score) {
  Model model;

  model.Start();
  model.UpdateState();
  model.MoveLeft();
  model.MoveSnake();
  model.MoveSnake();
  model.MoveSnake();
  model.MoveSnake();
  model.MoveLeft();
  model.MoveSnake();

  GameInfo_t info = model.GetCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 1);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[5][5], 0);
  EXPECT_NE(info.field[5][6], 0);
  EXPECT_NE(info.field[6][6], 0);
  EXPECT_NE(info.field[7][6], 0);
  EXPECT_NE(info.field[8][6], 0);

  bool new_eat = false;
  for (auto i = 0; i < HEIGHT; i++) {
    for (auto j = 0; j < WIDTH; j++) {
      if (info.field[i][j] == 2) {
        new_eat = true;
      }
    }
  }

  EXPECT_EQ(new_eat, true);
}

TEST(Controller, UserInputStart) {
  Controller *controller = GetController();
  controller->Clear();

  UserInput(Start, false);

  GameInfo_t info = UpdateCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);
  EXPECT_NE(info.field[9][3], 0);

  EXPECT_NE(info.field[5][5], 0);

  controller->Clear();
}

TEST(Controller, UserInputPause) {
  Controller *controller = GetController();
  controller->Clear();

  UserInput(Start, false);

  UpdateCurrentState();

  UserInput(Pause, false);
  GameInfo_t info = UpdateCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, true);

  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);
  EXPECT_NE(info.field[9][3], 0);

  EXPECT_NE(info.field[5][5], 0);

  controller->Clear();
}

TEST(Controller, UserInputAction) {
  Controller *controller = GetController();
  controller->Clear();

  UserInput(Start, false);

  UpdateCurrentState();

  UserInput(Action, false);
  GameInfo_t info = UpdateCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[9][7], 0);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);

  EXPECT_NE(info.field[5][5], 0);

  controller->Clear();
}

TEST(Controller, UserInputLeft) {
  Controller *controller = GetController();
  controller->Clear();

  UserInput(Start, false);

  UpdateCurrentState();
  controller->RefreshTimer();

  UserInput(Left, false);
  UserInput(Action, false);
  GameInfo_t info = UpdateCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[7][6], 0);
  EXPECT_NE(info.field[8][6], 0);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);

  EXPECT_NE(info.field[5][5], 0);

  controller->Clear();
}

TEST(Controller, UserInputRight) {
  Controller *controller = GetController();
  controller->Clear();

  UserInput(Start, false);

  UpdateCurrentState();
  controller->RefreshTimer();

  UserInput(Right, false);
  UserInput(Action, false);
  GameInfo_t info = UpdateCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, SPEED / 1000);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[11][6], 0);
  EXPECT_NE(info.field[10][6], 0);
  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);

  EXPECT_NE(info.field[5][5], 0);

  controller->Clear();
}

TEST(Controller, UserInputTerminate) {
  Controller *controller = GetController();
  controller->Clear();

  UserInput(Start, false);

  UpdateCurrentState();

  UserInput(Terminate, false);
  GameInfo_t info = UpdateCurrentState();

  EXPECT_NE(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.pause, false);

  EXPECT_NE(info.field[9][6], 0);
  EXPECT_NE(info.field[9][5], 0);
  EXPECT_NE(info.field[9][4], 0);
  EXPECT_NE(info.field[9][3], 0);

  EXPECT_NE(info.field[5][5], 0);

  UserInput(Terminate, false);
  info = UpdateCurrentState();

  EXPECT_EQ(info.field, nullptr);
  EXPECT_EQ(info.next, nullptr);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.speed, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.pause, false);

  controller->Clear();
}

TEST(Database, GetHighestScore) {
  Database db;

  db.CreateTable("test");

  db.InsertScore(101, "test");
  db.InsertScore(100, "test");

  EXPECT_EQ(db.GetHighestScore("test"), 101);
}

}  // namespace brick_game
