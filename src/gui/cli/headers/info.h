#ifndef SRC_GUI_CLI_INFO_H
#define SRC_GUI_CLI_INFO_H

#include "field.h"

#define OFFSET FIELD_WITH_BORDER_WIDTH + 4
#define FIGURE_BORDER_WIDTH CELL_WIDTH * 8
#define FIGURE_BORDER_HEIGHT CELL_HEIGHT * 8

#define CONTROLS_OFFSET_HEIGHT FIGURE_BORDER_HEIGHT + CELL_HEIGHT + 5

void print_next_figure(int **figure_matrix);
void print_control_keys(void);
void print_score(int highest_score, int score, int level, int speed);

#endif