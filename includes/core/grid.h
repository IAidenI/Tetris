#ifndef GRID_H
#define GRID_H

#include "core/game_settings.h"
#include "core/tetrominos.h"
#include "utils/tools.h"

typedef struct {
    int cell[GRID_WIDTH][GRID_HEIGHT];
} Grid;

void grid_init(Grid *g);
void grid_set_cell(Grid *g, Position p, int value);
int grid_apply_move(Grid *g, Tetromino *t);
int grid_apply_rotation(Grid *g, Tetromino *t);

#endif // GRID_H
