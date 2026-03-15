#ifndef GRID_H
#define GRID_H

#include "core/game_settings.h"
#include "core/tetrominos.h"
#include "utils/tools.h"
#include "utils/debug.h"

typedef enum {
    GRID_FAIL,
    GRID_OK,
    GRID_OUT_OF_BOUNDS,
    GRID_COLLISION
} GridCheck;

typedef struct {
    int cell[GRID_HEIGHT][GRID_WIDTH];
    int lines_cleared;
    int total_lines_cleared;
} Grid;

void grid_init(Grid *g);
void grid_set_cell(Grid *g, Position p, int value);
int  grid_apply_move(Grid *g, Tetromino *t, Position new_pos);
int  grid_apply_rotation(Grid *g, Tetromino *t, Action a);
void grid_lock_tetromino(Grid *g, Tetromino *t);
void grid_clear_full_lines(Grid *g);
GridCheck grid_check_position(Grid *g, Tetromino *t, Position p);
int grid_SRS(Grid *g, Tetromino *t, Action a);

#endif // GRID_H
