#ifndef BRICK_GAME_SNAKE_SNAKE_H
#define BRICK_GAME_SNAKE_SNAKE_H

#include <initializer_list>
#include <iostream>
#include <list>
#include <vector>

#include "../../../headers/game.h"

enum e_Direction { R, D, L, U };

namespace brick_game {

class Snake {
 public:
  Snake();
  ~Snake();
  Snake(const Snake &s);
  Snake &operator=(Snake &&s);

  void RotateSnake(int key);
  void Clear();
  void GetBody(int **field);
  int GetLength();
  bool Move(const int eat[2]);
  void Spawn();

 private:
  using value_type = std::pair<int, int>;
  std::list<value_type> body_;
  int direction_ = 0;
};

}  // namespace brick_game

#endif