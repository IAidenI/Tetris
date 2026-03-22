#ifndef GRID_H
#define GRID_H

#include "core/game_settings.h"
#include "core/tetrominos.h"
#include "utils/debug.h"

typedef enum {
    GRID_FAIL,
    GRID_OK,
    GRID_OUT_OF_BOUNDS,
    GRID_COLLISION
} GridCheck;

typedef struct {
    int dx;
    int dy;
} Kick;

typedef struct {
    int *cell;
    int width;
    int height;
    int lines_cleared;
    int total_lines_cleared;
} Grid;

void grid_init(Grid *g, Size grid_size);
int  grid_try_apply_move(Grid *g, Tetromino *t, Position new_pos);
void grid_lock_tetromino(Grid *g, Tetromino *t);
GridCheck grid_check_position(Grid *g, Tetromino *t, Position p);
int grid_SRS(Grid *g, Tetromino *t, Action a);
void grid_cleanup(Grid *g);

#endif // GRID_H
