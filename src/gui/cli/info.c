#include "./headers/info.h"

#include "./headers/colors.h"

void print_figure_cell(int x, int y) {
  for (int i = 0; i < CELL_HEIGHT; i++) {
    for (int j = 0; j < CELL_WIDTH; j++) {
      mvaddch(2 * CELL_HEIGHT + y * CELL_HEIGHT + i,
              OFFSET + 2 * CELL_WIDTH + x * CELL_WIDTH + j, ' ');
    }
  }
}

void print_next_figure_border(void) {
  attron(COLOR_PAIR(Magenta));
  for (int i = 0; i < FIGURE_BORDER_WIDTH; i++) {
    for (int j = 0; j < BORDER_HEIGHT; j++) {
      mvaddch(0, OFFSET + i, ' ');
    }
  }
  for (int i = 1; i < FIGURE_BORDER_HEIGHT; i++) {
    for (int j = 0; j < BORDER_WIDTH; j++) {
      mvaddch(i, OFFSET + j, ' ');
      mvaddch(i, OFFSET + FIGURE_BORDER_WIDTH - BORDER_WIDTH + j, ' ');
    }
  }
  for (int i = 0; i < FIGURE_BORDER_WIDTH; i++) {
    for (int j = 0; j < BORDER_HEIGHT; j++) {
      mvaddch(FIGURE_BORDER_HEIGHT - BORDER_HEIGHT, OFFSET + i, ' ');
    }
  }
  attroff(COLOR_PAIR(Magenta));
}

void print_next_figure(int **figure_matrix) {
  print_next_figure_border();
  attron(COLOR_PAIR(Green));
  for (int row = 0; row < 4; row++) {
    for (int column = 0; column < 4; column++) {
      if (figure_matrix[row][column]) {
        print_figure_cell(column, row);
      }
    }
  }
  attroff(COLOR_PAIR(Green));
}

void print_score(int highest_score, int score, int level, int speed) {
  mvprintw(FIGURE_BORDER_HEIGHT + CELL_HEIGHT, OFFSET, "Record: %d",
           highest_score);
  mvprintw(FIGURE_BORDER_HEIGHT + CELL_HEIGHT + 1, OFFSET, "Score: %d", score);
  mvprintw(FIGURE_BORDER_HEIGHT + CELL_HEIGHT + 2, OFFSET, "Level: %d", level);
  mvprintw(FIGURE_BORDER_HEIGHT + CELL_HEIGHT + 3, OFFSET, "Speed: %d ms",
           speed);
}

void print_control_keys(void) {
  mvprintw(CONTROLS_OFFSET_HEIGHT, OFFSET, "CONTROLS");
  mvprintw(CONTROLS_OFFSET_HEIGHT + 1, OFFSET, "Move left: %lc", L'←');
  mvprintw(CONTROLS_OFFSET_HEIGHT + 2, OFFSET, "Move right: %lc", L'→');
  mvprintw(CONTROLS_OFFSET_HEIGHT + 3, OFFSET, "Move down: %lc", L'↓');
  mvprintw(CONTROLS_OFFSET_HEIGHT + 4, OFFSET, "Move up: %lc", L'↑');
  mvprintw(CONTROLS_OFFSET_HEIGHT + 5, OFFSET, "Action: E");
  mvprintw(CONTROLS_OFFSET_HEIGHT + 6, OFFSET, "Pause: P");
  mvprintw(CONTROLS_OFFSET_HEIGHT + 7, OFFSET, "Stop game: SPACE");
}