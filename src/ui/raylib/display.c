#include "ui/raylib/display.h"

static void display_mino(Position p, Color c) {
    Rectangle rect = {
        (float)p.x,
        (float)p.y,
        (float)CELL_WIDTH,
        (float)CELL_HEIGHT
    };
    DrawRectangleRec(rect, c);
}

void display_tetromino(const Tetromino *t, Position grid_origin) {
    Position base = {
        grid_origin.x + t->pos.x * (CELL_WIDTH + GRID_THIN_BORDER),
        grid_origin.y + t->pos.y * (CELL_HEIGHT + GRID_THIN_BORDER)
    };

    /*Rectangle rect = {
        (float)base.x - GRID_THIN_BORDER, (float)base.y - GRID_THIN_BORDER,
        t->size * CELL_WIDTH  + (t->size + 1) * GRID_THIN_BORDER,
        t->size * CELL_HEIGHT + (t->size + 1) * GRID_THIN_BORDER
    };
    DrawRectangleLinesEx(rect, GRID_THIN_BORDER, COLOR_SPLIT);*/

    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) {
                Position p = {
                    base.x + w * (CELL_WIDTH + GRID_THIN_BORDER),
                    base.y + h * (CELL_HEIGHT + GRID_THIN_BORDER)
                };
                display_mino(p, color_get(t->type));
            }
        }
    }
}

void display_grid(const Grid *g, Position p) {
    int total_w = 2 * GRID_LARGE_BORDER + GRID_WIDTH * CELL_WIDTH + (GRID_WIDTH - 1) * GRID_THIN_BORDER;
    int total_h = 2 * GRID_LARGE_BORDER + GRID_HEIGHT * CELL_HEIGHT + (GRID_HEIGHT - 1) * GRID_THIN_BORDER;

    int inner_x = p.x + GRID_LARGE_BORDER;
    int inner_y = p.y + GRID_LARGE_BORDER;

    Rectangle outer = {
        (float)p.x, (float)p.y,
        (float)total_w, (float)total_h
    };
    DrawRectangleRec(outer, COLOR_SPLIT);

    for (int h = 0; h < GRID_HEIGHT; h++) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            Position cell_pos = {
                inner_x + w * (CELL_WIDTH + GRID_THIN_BORDER),
                inner_y + h * (CELL_HEIGHT + GRID_THIN_BORDER)
            };
            display_mino(cell_pos, color_get(g->cell[h][w]));
        }
    }
}
