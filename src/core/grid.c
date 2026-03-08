#include "core/grid.h"

void grid_init(Grid *g) {
    for (int w = 0; w < GRID_WIDTH; w++) {
        for (int h = 0; h < GRID_HEIGHT; h++) {
            g->cell[w][h] = 0;
        }
    }
}

void grid_set_cell(Grid *g, Position p, int value) {
    g->cell[p.x][p.y] = value;
}

int grid_apply_move(Grid *g, Tetromino *t) {
    if (t->next_pos.x == -1 || t->next_pos.y == -1) return 0;

    // Clear 
    for (int w = t->pos.x; w < t->pos.x + t->size; w++) {
        for (int h = t->pos.y; h < t->pos.y + t->size; h++) {
            if (t->shape[h - t->pos.y][w - t->pos.x]) {
                grid_set_cell(g, (Position){w, h}, __);
            }
        }
    }

    // Place new position
    for (int w = t->next_pos.x; w < t->next_pos.x + t->size; w++) {
        for (int h = t->next_pos.y; h < t->next_pos.y + t->size; h++) {
            if (t->shape[h - t->next_pos.y][w - t->next_pos.x]) {
                grid_set_cell(g, (Position){w, h}, t->shape[h - t->next_pos.y][w - t->next_pos.x]);
            }
        }
    }

    // Reset
    t->pos = t->next_pos;
    t->next_pos.x = -1;
    t->next_pos.y = -1;
    return 1;
}

int grid_apply_rotation(Grid *g, Tetromino *t) {
    // Check if present
    int has_next_shape = 0;
    for (int h = 0; h < t->size && !has_next_shape; h++) {
        for (int w = 0; w < t->size && !has_next_shape; w++) {
            if (t->next_shape[h][w]) has_next_shape = 1;
        }
    }

    if (!has_next_shape) return 0;

    // Clear
    for (int w = t->pos.x; w < t->pos.x + t->size; w++) {
        for (int h = t->pos.y; h < t->pos.y + t->size; h++) {
            if (t->shape[h - t->pos.y][w - t->pos.x]) grid_set_cell(g, (Position){w, h}, __);
        }
    }

    // Change shape
    memcpy(t->shape, t->next_shape, sizeof(t->next_shape));
    memset(t->next_shape, 0, sizeof(t->next_shape));

    // Redraw
    for (int w = t->pos.x; w < t->pos.x + t->size; w++) {
        for (int h = t->pos.y; h < t->pos.y + t->size; h++) {
            if (t->shape[h - t->pos.y][w - t->pos.x]) grid_set_cell(g, (Position){w, h}, t->shape[h - t->pos.y][w - t->pos.x]);
        }
    }
    return 1;
}
