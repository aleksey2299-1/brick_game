#include "./headers/controller.h"

namespace brick_game {

Controller::Controller() = default;

Controller::~Controller() { game_model_.Clear(); }

void Controller::Clear() { game_model_.Clear(); }

void Controller::RefreshTimer() { game_model_.RefreshTimer(); }

void Controller::UserInput(UserAction_t action, bool) {
  switch (action) {
    case Start:
      game_model_.Start();
      break;
    case Pause:
      game_model_.PauseGame();
      break;
    case Terminate:
      game_model_.GameOver();
      break;
    case Left:
      game_model_.MoveLeft();
      break;
    case Right:
      game_model_.MoveRight();
      break;
    case Action:
      game_model_.MoveSnake();
    default:
      break;
  }
}

GameInfo_t Controller::UpdateCurrentState(void) {
  game_model_.UpdateState();
  GameInfo_t info = game_model_.GetCurrentState();

  return info;
}

}  // namespace brick_game

brick_game::Controller *GetController() {
  static brick_game::Controller controller;
  return &controller;
}

GameInfo_t UpdateCurrentState(void) {
  brick_game::Controller *controller = GetController();
  return controller->UpdateCurrentState();
}

void UserInput(UserAction_t action, bool hold) {
  brick_game::Controller *controller = GetController();
  controller->UserInput(action, hold);
}
