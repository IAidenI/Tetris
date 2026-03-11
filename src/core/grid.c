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

GridCheck grid_check_position(Grid *g, Tetromino *t) {
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) {
                int gx = t->next_pos.x + w;
                int gy = t->next_pos.y + h;

                log_write("Position X=%d;Y=%d\n", gx, gy);

                // Check bounds
                if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT) return GRID_OUT_OF_BOUNDS;

                // Check collisons
                if (g->cell[gx][gy] != 0) return GRID_COLLISION;
            }
        }
    }
    return GRID_OK;
}

GridCheck grid_check_shape(const Grid *g, const Tetromino *t) {
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->next_shape[h][w] == 0)
                continue;

            int gx = t->pos.x + w;
            int gy = t->pos.y + h;

            // Check bounds
            if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT) return GRID_OUT_OF_BOUNDS;

            // Check collisions
            if (g->cell[gx][gy] != 0) return GRID_COLLISION;
        }
    }
    return GRID_OK;
}

int grid_apply_move(Grid *g, Tetromino *t) {
    if (t->next_pos.x == -99 || t->next_pos.y == -99) return GRID_IDLE;

    GridCheck result = grid_check_position(g, t);

    if (result == GRID_OK) t->pos = t->next_pos;

    t->next_pos = (Position){-99, -99};
    return result;
}

int grid_apply_rotation(Grid *g, Tetromino *t) {
    int has_next_shape = 0;

    for (int h = 0; h < t->size && !has_next_shape; h++) {
        for (int w = 0; w < t->size && !has_next_shape; w++) {
            if (t->next_shape[h][w] != 0)
                has_next_shape = 1;
        }
    }

    if (!has_next_shape) return GRID_IDLE;

    GridCheck result = grid_check_shape(g, t);

    if (result == GRID_OK) memcpy(t->shape, t->next_shape, sizeof(t->shape));
    memset(t->next_shape, 0, sizeof(t->next_shape));
    return result;
}

void grid_lock_tetromino(Grid *g, Tetromino *t) {
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) g->cell[t->pos.x + w][t->pos.y + h] = t->shape[h][w];
        }
    }
}
