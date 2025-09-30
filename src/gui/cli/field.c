#include "./headers/field.h"

#include "./headers/colors.h"

void print_field_border(void) {
  attron(COLOR_PAIR(Blue));
  for (int i = 0; i < FIELD_WITH_BORDER_WIDTH; i++) {
    for (int j = 0; j < BORDER_HEIGHT; j++) {
      mvaddch(0, i, ' ');
    }
  }
  for (int i = 1; i < FIELD_WITH_BORDER_HEIGHT; i++) {
    for (int j = 0; j < BORDER_WIDTH; j++) {
      mvaddch(i, j, ' ');
      mvaddch(i, FIELD_WITH_BORDER_WIDTH - BORDER_WIDTH + j, ' ');
    }
  }
  for (int i = 0; i < FIELD_WITH_BORDER_WIDTH; i++) {
    for (int j = 0; j < BORDER_HEIGHT; j++) {
      mvaddch(HEIGHT + 1, i, ' ');
    }
  }
  attroff(COLOR_PAIR(Blue));
}

void print_cell(int y, int x) {
  for (int i = 0; i < CELL_HEIGHT; i++) {
    for (int j = 0; j < CELL_WIDTH; j++) {
      mvaddch(BORDER_HEIGHT + y * CELL_HEIGHT + i,
              BORDER_WIDTH + x * CELL_WIDTH + j, ' ');
    }
  }
}

void print_field(int **field) {
  for (int row = 0; row < HEIGHT; row++) {
    for (int column = 0; column < WIDTH; column++) {
      if (field[row][column]) {
        if (field[row][column] == 1) {
          attron(COLOR_PAIR(Yellow));
        } else if (field[row][column] == 2) {
          attron(COLOR_PAIR(Green));
        } else if (field[row][column] == 3) {
          attron(COLOR_PAIR(Magenta));
        } else if (field[row][column] == 4) {
          attron(COLOR_PAIR(Red));
        } else if (field[row][column] > 4) {
          attron(COLOR_PAIR(Green));
        }
        print_cell(row, column);
        attroff(COLOR_PAIR(Magenta));
        attroff(COLOR_PAIR(Yellow));
        attroff(COLOR_PAIR(Green));
        attroff(COLOR_PAIR(Red));
      }
    }
  }
}

void print_game_over(void) {
  static const int game_over[12][22] = GAME_OVER_PRINT;
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 22; j++) {
      if (game_over[i][j]) {
        attron(COLOR_PAIR(Red));
        print_cell(CELL_HEIGHT + i, CELL_WIDTH + j);
        attroff(COLOR_PAIR(Red));
      } else {
        attron(COLOR_PAIR(Black));
        print_cell(CELL_HEIGHT + i, CELL_WIDTH + j);
        attroff(COLOR_PAIR(Black));
      }
    }
  }
  move(BORDER_HEIGHT + 11 * CELL_HEIGHT + 1,
       BORDER_WIDTH + 8 * CELL_WIDTH + CELL_WIDTH / 2);
  char *str = "Press 'R' to restart.";
  for (; *str; str++) {
    addch(*str | A_BOLD | A_UNDERLINE);
  }
}

void print_victory(void) {
  static const int victory[5][39] = VICTORY_PRINT;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 39; j++) {
      if (victory[i][j]) {
        attron(COLOR_PAIR(Green));
        print_cell(CELL_HEIGHT + i, CELL_WIDTH + j);
        attroff(COLOR_PAIR(Green));
      } else {
        attron(COLOR_PAIR(Black));
        print_cell(CELL_HEIGHT + i, CELL_WIDTH + j);
        attroff(COLOR_PAIR(Black));
      }
    }
  }
  move(BORDER_HEIGHT + 5 * CELL_HEIGHT + 1,
       BORDER_WIDTH + 16 * CELL_WIDTH + CELL_WIDTH / 2);
  char *str = "Press 'R' to restart.";
  for (; *str; str++) {
    addch(*str | A_BOLD | A_UNDERLINE);
  }
}

void print_pause(void) {
  static const int pause[5][32] = PAUSED_PRINT;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 32; j++) {
      if (pause[i][j]) {
        attron(COLOR_PAIR(White));
        print_cell(CELL_HEIGHT + i, CELL_WIDTH + j);
        attroff(COLOR_PAIR(White));
      } else {
        attron(COLOR_PAIR(Black));
        print_cell(CELL_HEIGHT + i, CELL_WIDTH + j);
        attroff(COLOR_PAIR(Black));
      }
    }
  }
  move(BORDER_HEIGHT + 5 * CELL_HEIGHT + 1,
       BORDER_WIDTH + 12 * CELL_WIDTH + CELL_WIDTH / 2);
  char *str = "Press 'P' to continue.";
  for (; *str; str++) {
    addch(*str | A_BOLD | A_UNDERLINE);
  }
}