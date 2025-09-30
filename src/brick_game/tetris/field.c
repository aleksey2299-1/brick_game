#include <stdlib.h>

#include "../../headers/game.h"

void delete_row(int **field, int row_for_delete) {
  int prev_row[WIDTH] = {0};
  int current_row[WIDTH] = {0};
  for (int row = 0; row <= row_for_delete; row++) {
    for (int column = 0; column < WIDTH; column++) {
      current_row[column] = field[row][column];
      field[row][column] = prev_row[column];
      prev_row[column] = current_row[column];
    }
  }
}

int check_rows(int **field) {
  int cleaned = 0;
  for (int row = 0; row < HEIGHT; row++) {
    for (int column = 0; column < WIDTH && field[row][column]; column++) {
      if (column == WIDTH - 1) {
        delete_row(field, row);
        cleaned++;
      }
    }
  }
  return cleaned;
}

void create_field(int ***field) {
  if (*field == NULL) {
    *field = calloc(HEIGHT, sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
      (*field)[i] = calloc(WIDTH, sizeof(int));
    }
  }
}

void clear_field(int ***field) {
  if (*field != NULL) {
    for (int i = 0; i < HEIGHT; i++) {
      free((*field)[i]);
    }
    free(*field);
    *field = NULL;
  }
}
