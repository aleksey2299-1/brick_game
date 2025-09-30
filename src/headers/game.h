#ifndef SRC_HEADERS_GAME_H
#define SRC_HEADERS_GAME_H

#include <stdbool.h>

#define VICTORY -1
#define HEIGHT 20
#define WIDTH 10
#define SPEED 1000000
#define SPEED_BOOST SPEED / 10

typedef enum UserAction_t {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct GameInfo_t {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#ifdef __cplusplus
extern "C" {
#endif

void UserInput(UserAction_t action, bool hold);
GameInfo_t UpdateCurrentState(void);

#ifdef __cplusplus
}
#endif

#endif