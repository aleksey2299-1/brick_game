#ifndef BRICK_GAME_SNAKE_CONTROLLER_H
#define BRICK_GAME_SNAKE_CONTROLLER_H

#include "../../../headers/game.h"
#include "model.h"

namespace brick_game {

class Controller {
 public:
  Controller();
  ~Controller();

  void UserInput(UserAction_t action, bool hold);
  GameInfo_t UpdateCurrentState(void);
  void Clear();
  void RefreshTimer();

 private:
  Model game_model_;
};

}  // namespace brick_game

brick_game::Controller *GetController();

#endif