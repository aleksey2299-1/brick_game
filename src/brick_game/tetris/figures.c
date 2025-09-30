#include "./headers/figures.h"

#include "../../headers/game.h"

int check_spawn_point(figure_t *figure, int **field) {
  int allow = 1;
  int matrix[4][4];
  get_figure_matrix(figure, matrix);
  for (int row = 0; row < 4 && allow; row++) {
    for (int column = 0; column < 4 && allow; column++) {
      int cell =
          field[figure->position[0] + row][figure->position[1] + column] +
          matrix[row][column];
      if (cell > 1) {
        allow = 0;
      }
    }
  }
  return allow;
}

void change_orientation(figure_t *figure) {
  int orientation = figure->orientation;
  if (orientation == ROTATION_270) {
    figure->orientation = ROTATION_0;
  } else {
    figure->orientation = orientation + 1;
  }
}

int check_rotation(const figure_t *figure, int **field) {
  int allow = 1;
  int matrix[4][4];
  figure_t rotated_figure = *figure;
  change_orientation(&rotated_figure);
  get_figure_matrix(&rotated_figure, matrix);
  int cell = 0;
  for (int row = 0; row < 4 && allow; row++) {
    for (int column = 0; column < 4 && allow; column++) {
      if (matrix[row][column]) {
        if (rotated_figure.position[0] + row >= HEIGHT ||
            rotated_figure.position[1] + column >= WIDTH) {
          allow = 0;
        } else {
          cell = field[rotated_figure.position[0] + row]
                      [rotated_figure.position[1] + column] +
                 matrix[row][column];
        }
      }
      if (cell > 1) {
        allow = 0;
      }
    }
  }
  return allow;
}

int spawn_figure(figure_t *figure, int type, int **field) {
  int result = 0;
  figure->orientation = ROTATION_0;
  figure->position[0] = 0;
  figure->position[1] = 4;
  figure->type = type;
  figure->status = UNFIXED;
  if (!check_spawn_point(figure, field)) {
    result = 1;
  }
  return result;
}

void rotate_figure(figure_t *figure, int **field) {
  if (check_rotation(figure, field)) {
    change_orientation(figure);
  }
}

void fill_figure_matrix(int matrix[4][4], const int macro_matrix[4][4][4],
                        int orientation) {
  for (int row = 0; row < 4; row++) {
    for (int column = 0; column < 4; column++) {
      matrix[row][column] = macro_matrix[orientation][row][column];
    }
  }
}

void get_figure_matrix(const figure_t *figure, int matrix[4][4]) {
  switch (figure->type) {
    case O: {
      static const int data[4][4][4] = O_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    case J: {
      static const int data[4][4][4] = J_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    case L: {
      static const int data[4][4][4] = L_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    case S: {
      static const int data[4][4][4] = S_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    case Z: {
      static const int data[4][4][4] = Z_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    case T: {
      static const int data[4][4][4] = T_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    case I: {
      static const int data[4][4][4] = I_FIGURE;
      fill_figure_matrix(matrix, data, figure->orientation);
      break;
    }
    default:
      break;
  }
}

int check_left(figure_t *figure, int **field) {
  int allow = 1;
  int matrix[4][4];
  get_figure_matrix(figure, matrix);
  int cell = 0;
  if (figure->position[1] - 1 < 0) {
    allow = 1;
  }
  for (int row = 0; row < 4 && allow; row++) {
    for (int column = 0; column < 4 && allow; column++) {
      if (matrix[row][column]) {
        if (figure->position[1] + column - 1 < 0) {
          allow = 0;
        } else {
          cell = field[figure->position[0] + row]
                      [figure->position[1] + column - 1] +
                 matrix[row][column];
        }
      }
      if (cell > 1) {
        allow = 0;
      }
    }
  }
  return allow;
}

int check_right(figure_t *figure, int **field) {
  int allow = 1;
  int matrix[4][4];
  get_figure_matrix(figure, matrix);
  int cell = 0;
  for (int row = 0; row < 4 && allow; row++) {
    for (int column = 0; column < 4 && allow; column++) {
      if (matrix[row][column]) {
        if (figure->position[1] + column + 1 >= WIDTH) {
          allow = 0;
        } else {
          cell = field[figure->position[0] + row]
                      [figure->position[1] + column + 1] +
                 matrix[row][column];
        }
      }
      if (cell > 1) {
        allow = 0;
      }
    }
  }
  return allow;
}

int check_down(figure_t *figure, int **field) {
  int allow = 1;
  int matrix[4][4];
  get_figure_matrix(figure, matrix);
  int cell = 0;
  for (int row = 0; row < 4 && allow; row++) {
    for (int column = 0; column < 4 && allow; column++) {
      if (matrix[row][column]) {
        if (figure->position[0] + row + 1 >= HEIGHT) {
          allow = 0;
        } else {
          cell = field[figure->position[0] + row + 1]
                      [figure->position[1] + column] +
                 matrix[row][column];
        }
      }
      if (cell > 1) {
        allow = 0;
      }
    }
  }
  return allow;
}

int shift(figure_t *figure, int direction, int **field) {
  int result = 0;
  switch (direction) {
    case LEFT: {
      if (check_left(figure, field)) {
        figure->position[1] -= 1;
      } else {
        result = 1;
      }
      break;
    }
    case RIGHT: {
      if (check_right(figure, field)) {
        figure->position[1] += 1;
      } else {
        result = 1;
      }
      break;
    }
    case DOWN: {
      if (check_down(figure, field)) {
        figure->position[0] += 1;
      } else {
        figure->status = FIXED;
        result = 1;
      }
      break;
    }
    default:
      break;
  }
  return result;
}

int move_figure(figure_t *figure, int direction, int **field) {
  int result = 0;
  if (figure->status == UNFIXED) {
    result = shift(figure, direction, field);
  }
  return result;
}

void add_figure_to_field(figure_t *figure, int **field, int value) {
  int matrix[4][4];
  get_figure_matrix(figure, matrix);
  for (int row = 0; row < 4; row++) {
    for (int column = 0; column < 4; column++) {
      if (matrix[row][column]) {
        field[figure->position[0] + row][figure->position[1] + column] +=
            matrix[row][column] * value;
      }
    }
  }
}