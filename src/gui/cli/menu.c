#include <ncurses.h>

#include "./headers/colors.h"

void displayMenu(int position) {
  clear();
  mvprintw(0, 0, "Choose game");
  if (position == 0) {
    attron(COLOR_PAIR(BlackOnWhite));
  }
  mvprintw(3, 0, "Tetris");
  attroff(COLOR_PAIR(BlackOnWhite));
  if (position == 1) {
    attron(COLOR_PAIR(BlackOnWhite));
  }
  mvprintw(4, 0, "Snake");
  attroff(COLOR_PAIR(BlackOnWhite));
  if (position == 2) {
    attron(COLOR_PAIR(BlackOnWhite));
  }
  mvprintw(5, 0, "Race");
  attroff(COLOR_PAIR(BlackOnWhite));
  refresh();
}

int menu() {
  int position = 0;
  while (1) {
    int x = getch();
    switch (x) {
      case KEY_DOWN:
        position = (position + 1) % 3;
        break;
      case KEY_UP:
        position = (position - 1) % 3;
        if (position < 0) {
          position = 3 + position;
        }
        break;
      case ' ':
        return -1;
      case 'r':
      case 'R':
        return position;
    }
    displayMenu(position);
  }
}
