#ifndef DISPLAY_H
#define DISPLAY_H

#include <raylib.h>
#include "utils/types.h"
#include "ui/raylib/colors.h"
#include "ui/raylib/component.h"
#include "ui/raylib/manager.h"
#include "core/game.h"

#define CELL_WIDTH  40
#define CELL_HEIGHT 40
#define GRID_THIN_BORDER  2.0f
#define GRID_LARGE_BORDER 4.0f

static inline Size display_get_grid_size(const Grid *g) {
    return (Size){
        .w = 2 * GRID_LARGE_BORDER + g->width  * CELL_WIDTH  + (g->width  - 1) * GRID_THIN_BORDER,
        .h = 2 * GRID_LARGE_BORDER + g->height * CELL_HEIGHT + (g->height - 1) * GRID_THIN_BORDER
    };
}

static inline Size display_tetromino_widget_get_size(const TextStyle label, const Tetromino *t) {
    Vector2 label_size = MeasureTextStyled(label);
    int padY = 10;

    Grid fake = {0};
    fake.width = t->size;
    fake.height = t->size;
    Size grid_size = display_get_grid_size(&fake);

    return (Size){
        .w = grid_size.w,
        .h = (int)label_size.y + padY + grid_size.h
    };
}

void display_render(const Game *g, const Manager *m);

#endif // DISPLAY_H