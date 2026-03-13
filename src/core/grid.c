#include "core/grid.h"

void grid_init(Grid *g) {
    for (int w = 0; w < GRID_WIDTH; w++) {
        for (int h = 0; h < GRID_HEIGHT; h++) {
            g->cell[w][h] = 0;
        }
    }

    g->lines_cleared       = 0;
    g->total_lines_cleared = 0;
}

void grid_set_cell(Grid *g, Position p, int value) {
    g->cell[p.x][p.y] = value;
}

GridCheck grid_check_next_position(Grid *g, Tetromino *t) {
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[w][h]) {
                int gx = t->next_pos.x + w;
                int gy = t->next_pos.y + h;

                // Check bounds
                if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT) return GRID_OUT_OF_BOUNDS;

                // Check collisons
                if (g->cell[gx][gy] != 0) return GRID_COLLISION;
            }
        }
    }
    return GRID_OK;
}

GridCheck grid_check_next_shape(const Grid *g, const Tetromino *t) {
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->next_shape[w][h] == 0)
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
    if (t->next_pos.x == RESET_POSITION.x || t->next_pos.y == RESET_POSITION.y) return GRID_IDLE;

    GridCheck result = grid_check_next_position(g, t);

    if (result == GRID_OK) t->pos = t->next_pos;

    t->next_pos = RESET_POSITION;
    return result;
}

int grid_apply_rotation(Grid *g, Tetromino *t) {
    int has_next_shape = 0;

    for (int h = 0; h < t->size && !has_next_shape; h++) {
        for (int w = 0; w < t->size && !has_next_shape; w++) {
            if (t->next_shape[w][h] != 0)
                has_next_shape = 1;
        }
    }

    if (!has_next_shape) return GRID_IDLE;

    GridCheck result = grid_check_next_shape(g, t);

    if (result == GRID_OK) memcpy(t->shape, t->next_shape, sizeof(t->shape));
    memset(t->next_shape, 0, sizeof(t->next_shape));
    return result;
}

void grid_lock_tetromino(Grid *g, Tetromino *t) {
    // Lock tetromino in the grid when no move possible
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[w][h]) g->cell[t->pos.x + w][t->pos.y + h] = t->shape[w][h];
        }
    }

    grid_clear_full_lines(g);
}

void grid_clear_full_lines(Grid *g) {
    int write_h = GRID_HEIGHT - 1;

    for (int read_h = GRID_HEIGHT - 1; read_h >= 0; read_h--) {
        int line_full = 1;

        // Check if line is full
        for (int w = 0; w < GRID_WIDTH; w++) {
            if (g->cell[w][read_h] == 0) {
                line_full = 0;
                break;
            }
        }


        // If unfull line, copy below
        if (!line_full) {
            for (int w = 0; w < GRID_WIDTH; w++) {
                g->cell[w][write_h] = g->cell[w][read_h];
            }
            write_h--;
        }
    }

    // Clear all remaning lines
    for (int h = write_h; h >= 0; h--) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            g->cell[w][h] = __;
        }
    }

    int cleared = write_h + 1;
    g->lines_cleared        = cleared;
    g->total_lines_cleared += cleared;
}
