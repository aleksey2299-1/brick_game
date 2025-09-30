#include "./headers/interface.h"

#include <dlfcn.h>
#include <locale.h>
#include <ncurses.h>

#include "../../headers/game.h"
#include "./headers/colors.h"
#include "./headers/field.h"
#include "./headers/info.h"
#include "./headers/menu.h"

int user_action(int action);
void display(GameInfo_t game_info);
void inizialize_ncurses(void);
int loadLibrary(const char *name, void **handle, void (**input)(int, bool),
                GameInfo_t (**update)());

int main() {
  static const char *libraries[3] = LIBRARIES;
  int stop = 0;
  int initial = 1;
  inizialize_ncurses();
  GameInfo_t game_info;
  void *handle;
  GameInfo_t (*update)();
  void (*input)(int, bool);
  int x = -1;
  while (stop != 1) {
    if (initial) {
      int library = menu();
      if (library == -1) {
        stop = 1;
      } else {
        if (loadLibrary(libraries[library], &handle, &input, &update)) {
          stop = 1;
        };
        initial = 0;
      }
    } else {
      int prev_x = x;
      x = user_action(getch());
      game_info = update();
      if (game_info.field) {
        display(game_info);
      }
      if (prev_x == x && x != -1) {
        input(x, true);
      } else {
        input(x, false);
      }
      if ((game_info.speed == 0 || game_info.level == VICTORY) &&
          x == Terminate) {
        initial = 1;
        dlclose(handle);
      }
    }
  }
  endwin();
  return 0;
}

void inizialize_ncurses(void) {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  timeout(100);
  init_pair(Blue, COLOR_BLUE, COLOR_BLUE);
  init_pair(Magenta, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(Yellow, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(Green, COLOR_GREEN, COLOR_GREEN);
  init_pair(Red, COLOR_RED, COLOR_RED);
  init_pair(Black, COLOR_BLACK, COLOR_BLACK);
  init_pair(White, COLOR_WHITE, COLOR_WHITE);
  init_pair(BlackOnWhite, COLOR_BLACK, COLOR_WHITE);
}

int user_action(int action) {
  int action_code = -1;
  switch (action) {
    case 'p':
    case 'P':
      action_code = Pause;
      break;
    case 'r':
    case 'R':
      action_code = Start;
      break;
    case 'e':
    case 'E':
      action_code = Action;
      break;
    case KEY_LEFT:
      action_code = Left;
      break;
    case KEY_RIGHT:
      action_code = Right;
      break;
    case KEY_DOWN:
      action_code = Down;
      break;
    case KEY_UP:
      action_code = Up;
      break;
    case ' ':
      action_code = Terminate;
      break;
  }
  return action_code;
}

void display(GameInfo_t game_info) {
  clear();
  if (game_info.field != NULL) {
    print_field_border();
    print_score(game_info.high_score, game_info.score, game_info.level,
                game_info.speed);
    print_control_keys();
    print_field(game_info.field);
    if (game_info.next != NULL) {
      print_next_figure(game_info.next);
    }
    if (game_info.speed == 0) {
      print_game_over();
    }
    if (game_info.level == VICTORY) {
      print_victory();
    }
    if (game_info.pause) {
      print_pause();
    }
  }
  refresh();
}

int loadLibrary(const char *name, void **handle, void (**input)(int, bool),
                GameInfo_t (**update)()) {
  const char *error;
  *handle = dlopen(name, RTLD_LAZY);
  if (!*handle) {
    fprintf(stderr, "%s\n", dlerror());
    return 1;
  }
  dlerror();
  *input = dlsym(*handle, "UserInput");
  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    return 1;
  }

  *update = dlsym(*handle, "UpdateCurrentState");
  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    return 1;
  }
  (*input)(Start, false);
  return 0;
}