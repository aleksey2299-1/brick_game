#include "./headers/game.h"

#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "./headers/db.h"
#include "./headers/field.h"
#include "./headers/figures.h"

void clear_figure_matrix(int ***matrix) {
  if (*matrix != NULL) {
    for (int i = 0; i < 4; i++) {
      free((*matrix)[i]);
    }
    free(*matrix);
    *matrix = NULL;
  }
}

game_info_t *get_game_info(void) {
  static game_info_t game_info;
  return &game_info;
}

GameInfo_t *get_game_info_for_front(void) {
  static GameInfo_t game_info;
  return &game_info;
}

int spawn_new_figure(game_info_t *game_info) {
  int stop = 0;
  if (game_info->state == SPAWNING) {
    stop = spawn_figure(&game_info->figure, game_info->next_figure,
                        game_info->field);
    int next_figure = rand() % FIGURES_COUNT;
    game_info->next_figure = next_figure;
  }
  return stop;
}

void stop_game(void) {
  game_info_t *game_info = get_game_info();
  if (game_info->state == GAMEOVER) {
    add_score(game_info->score);
    game_info->state = WAITING;
    game_info->pause = 0;
  }
}

void moving(void) {
  game_info_t *game_info = get_game_info();
  if (game_info->state == MOVING) {
    if (!game_info->pause) {
      int status = move_figure(&game_info->figure, DOWN, game_info->field);
      if (status) {
        game_info->state = ATTACHING;
      }
    }
  }
}

void spawning(void) {
  game_info_t *game_info = get_game_info();
  if (!game_info->pause && game_info->state == SPAWNING) {
    int status = spawn_new_figure(game_info);
    if (status) {
      game_info->state = GAMEOVER;
    } else {
      game_info->state = MOVING;
    }
  }
}

int calc_score(int cleared) {
  int result = 0;
  switch (cleared) {
    case 1:
      result = 100;
      break;
    case 2:
      result = 300;
      break;
    case 3:
      result = 700;
      break;
    case 4:
      result = 1500;
      break;
    default:
      break;
  }
  return result;
}

void lvl_up(game_info_t *game_info) {
  int score = game_info->score;
  if (game_info->level != score / 600 + 1) {
    if (score >= 5400) {
      game_info->level = 10;
    } else {
      game_info->level = score / 600 + 1;
    }
    game_info->speed = SPEED - SPEED_BOOST * (game_info->level - 1);
  }
}

void attaching(void) {
  game_info_t *game_info = get_game_info();
  if (!game_info->pause && game_info->state == ATTACHING) {
    add_figure_to_field(&game_info->figure, game_info->field, 1);
    int cleared = check_rows(game_info->field);
    if (cleared) {
      int score = calc_score(cleared);
      game_info->score += score;
      lvl_up(game_info);
    }
    game_info->state = SPAWNING;
  }
}

long long current_microseconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

void start_game(void) {
  create_table();
  game_info_t *game_info = get_game_info();
  if (game_info->state == STARTING) {
    game_info->level = 1;
    game_info->speed = SPEED;
    game_info->high_score = get_highest_score();
    game_info->score = 0;
    game_info->pause = 0;
    game_info->state = SPAWNING;
  }
}

void move_left(void) {
  game_info_t *game_info = get_game_info();
  if (!game_info->pause && game_info->state == MOVING) {
    move_figure(&game_info->figure, LEFT, game_info->field);
  }
}

void move_right(void) {
  game_info_t *game_info = get_game_info();
  if (!game_info->pause && game_info->state == MOVING) {
    move_figure(&game_info->figure, RIGHT, game_info->field);
  }
}

void move_down(void) {
  game_info_t *game_info = get_game_info();
  if (!game_info->pause && game_info->state == MOVING) {
    if (game_info->figure.status == UNFIXED) {
      int status = move_figure(&game_info->figure, DOWN, game_info->field);
      if (status) {
        game_info->state = ATTACHING;
      }
    }
  }
}

void rotate(void) {
  game_info_t *game_info = get_game_info();
  if (!game_info->pause && game_info->state == MOVING) {
    rotate_figure(&game_info->figure, game_info->field);
  }
}

void pause_game(void) {
  game_info_t *game_info = get_game_info();
  if (game_info->state != GAMEOVER && game_info->state != WAITING) {
    if (game_info->pause) {
      game_info->pause = 0;
    } else {
      game_info->pause = 1;
    }
  }
}

void user_start(void) {
  game_info_t *game_info = get_game_info();
  if (game_info->field == NULL) {
    create_field(&game_info->field);
  }
  if (game_info->state == WAITING) {
    clear_field(&game_info->field);
    create_field(&game_info->field);
    game_info->state = STARTING;
  }
}

void user_stop(void) {
  game_info_t *game_info = get_game_info();
  GameInfo_t *gi = get_game_info_for_front();
  if (game_info->state != WAITING) {
    game_info->state = GAMEOVER;
    game_info->pause = 0;
  } else {
    clear_field(&game_info->field);
    clear_field(&gi->field);
    clear_figure_matrix(&gi->next);
  }
}

void moving_interval(void) {
  const game_info_t *game_info = get_game_info();
  static long long last_time = 0;
  long long now = current_microseconds();
  int tick_interval = game_info->speed;
  if (last_time == 0) {
    last_time = now;
  }

  if ((now - last_time) >= tick_interval) {
    last_time = now;
    moving();
  }
}

void change_state(int state) {
  switch (state) {
    case STARTING:
      start_game();
      break;
    case SPAWNING:
      spawning();
      break;
    case MOVING:
      moving_interval();
      break;
    case ATTACHING:
      attaching();
      break;
    case GAMEOVER:
      stop_game();
      break;
    default:
      break;
  }
}

void form_field(int ***field) {
  create_field(field);
  game_info_t *game_info = get_game_info();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      (*field)[i][j] = game_info->field[i][j];
    }
  }
  add_figure_to_field(&game_info->figure, *field, 2);
}

void form_next_figure(int next_figure, int ***matrix) {
  if (*matrix == NULL) {
    *matrix = calloc(4, sizeof(int *));
    for (int i = 0; i < 4; i++) {
      (*matrix)[i] = calloc(4, sizeof(int));
    }
  }
  figure_t figure = {0};
  figure.type = next_figure;
  int fig[4][4] = {0};
  get_figure_matrix(&figure, fig);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*matrix)[i][j] = fig[i][j];
    }
  }
}

void form_game_info(void) {
  const game_info_t *game_info = get_game_info();
  GameInfo_t *gi = get_game_info_for_front();
  if (game_info->field) {
    form_field(&gi->field);
  }
  if (!game_info->field) {
    clear_figure_matrix(&gi->next);
  } else {
    form_next_figure(game_info->next_figure, &gi->next);
  }
  if (game_info->score > game_info->high_score) {
    gi->high_score = game_info->score;
  } else {
    gi->high_score = game_info->high_score;
  }
  gi->score = game_info->score;
  gi->level = game_info->level;
  if (game_info->state == GAMEOVER || game_info->state == WAITING) {
    gi->speed = 0;
  } else {
    gi->speed = game_info->speed / 1000;
  }
  gi->pause = game_info->pause;
}

GameInfo_t UpdateCurrentState(void) {
  const game_info_t *game_info = get_game_info();

  change_state(game_info->state);

  form_game_info();

  return *get_game_info_for_front();
}

void UserInput(UserAction_t action, bool hold) {
  (void)hold;
  switch (action) {
    case Start:
      user_start();
      break;
    case Action:
      rotate();
      break;
    case Left:
      move_left();
      break;
    case Right:
      move_right();
      break;
    case Down:
      move_down();
      break;
    case Pause:
      pause_game();
      break;
    case Terminate:
      user_stop();
      break;
    default:
      break;
  }
}