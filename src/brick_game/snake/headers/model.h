#ifndef BRICK_GAME_SNAKE_MODEL_H
#define BRICK_GAME_SNAKE_MODEL_H

#include <chrono>
#include <random>

#include "../../../headers/game.h"
#include "db.h"
#include "snake.h"

enum e_States {
  STARTING,
  MOVING,
  MOVING_LEFT,
  MOVING_RIGHT,
  GAMEOVER,
  WAITING
};

namespace brick_game {

class Model {
 public:
  Model();
  ~Model();
  Model(const Model &m);
  Model &operator=(Model &&m);

  GameInfo_t GetCurrentState();
  void Clear();
  void GameOver();
  void PauseGame();
  void UpdateState();
  void MoveLeft();
  void MoveRight();
  void MoveSnake();
  void Start();
  void AddScore();
  void RefreshTimer();

 private:
  int64_t last_time_ = 0;
  std::mt19937 gen_;
  int state_;
  int speed_;
  int level_;
  int **field_ = nullptr;
  bool pause_;
  int score_;
  int high_score_;
  Snake snake_;
  int eat_[2] = {5, 5};

  void StartGame();
  void SpawnEat();
  void CheckEat();
  void ClearField();
  void CreateField();
  void LvlUp();
  void MoveInterval();
  void Victory();
  int64_t Now();
};

}  // namespace brick_game

#endif