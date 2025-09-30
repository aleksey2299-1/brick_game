#ifndef SRC_BRICK_GAME_TETRIS_GAME_H
#define SRC_BRICK_GAME_TETRIS_GAME_H

#include "../../../headers/game.h"
#include "figures.h"

typedef enum {
  WAITING,
  STARTING,
  SPAWNING,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
} e_states;

typedef struct GameInfoBackend_t {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  figure_t figure;
  int next_figure;
  int state;
} game_info_t;

game_info_t *get_game_info(void);
GameInfo_t *get_game_info_for_front(void);

#endif