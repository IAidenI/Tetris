#ifndef DISPLAY_H
#define DISPLAY_H

#include <raylib.h>
#include "utils/types.h"
#include "ui/raylib/colors.h"
#include "core/tetrominos.h"
#include "core/grid.h"

#define CELL_WIDTH  40
#define CELL_HEIGHT 40
#define GRID_THIN_BORDER  2.0f
#define GRID_LARGE_BORDER 4.0f

void display_tetromino(const Tetromino *t, Position grid_origin);
void display_grid(const Grid *g, Position p);

#endif // DISPLAY_H