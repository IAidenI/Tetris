#include "core/grid.h"

void grid_init(Grid *g) {
    for (int w = 0; w < GRID_WIDTH; w++) {
        for (int h = 0; h < GRID_HEIGHT; h++) {
            g->cell[h][w] = 0;
        }
    }

    g->lines_cleared       = 0;
    g->total_lines_cleared = 0;
}

void grid_set_cell(Grid *g, Position p, int value) {
    g->cell[p.y][p.x] = value;
}

int grid_apply_move(Grid *g, Tetromino *t, Position new_pos) {
    GridCheck result = grid_check_position(g, t, new_pos);
    if (result == GRID_OK) t->pos = new_pos;
    return result;
}

int grid_apply_rotation(Grid *g, Tetromino *t, Action a) {
    Tetromino rotated = *t;
    tetromino_rotate(&rotated, a);

    if (grid_check_position(g, &rotated, rotated.pos)) {
        *t = rotated;
        return GRID_OK;
    }
    return GRID_FAIL;
}

void grid_lock_tetromino(Grid *g, Tetromino *t) {
    // Lock tetromino in the grid when no move possible
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) g->cell[t->pos.y + h][t->pos.x + w] = t->shape[h][w];
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
            if (g->cell[read_h][w] == 0) {
                line_full = 0;
                break;
            }
        }

        // If unfull line, copy below
        if (!line_full) {
            for (int w = 0; w < GRID_WIDTH; w++) {
                g->cell[write_h][w] = g->cell[read_h][w];
            }
            write_h--;
        }
    }

    // Clear all remaning lines
    for (int h = write_h; h >= 0; h--) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            g->cell[h][w] = __;
        }
    }

    int cleared = write_h + 1;
    g->lines_cleared        = cleared;
    g->total_lines_cleared += cleared;
}

GridCheck grid_check_position(Grid *g, Tetromino *t, Position p) {
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) {
                int gx = p.x + w;
                int gy = p.y + h;

                // Check bounds
                if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT) return GRID_OUT_OF_BOUNDS;

                // Check collisons
                if (g->cell[gy][gx] != 0) return GRID_COLLISION;
            }
        }
    }
    return GRID_OK;
}

static const Kick JLSTZ_KICKS[4][4][5] = {
    [ROT_0][ROT_R] = {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}},
    [ROT_R][ROT_0] = {{0,0},{1,0},{1,-1},{0,2},{1,2}},

    [ROT_R][ROT_2] = {{0,0},{1,0},{1,-1},{0,2},{1,2}},
    [ROT_2][ROT_R] = {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}},

    [ROT_2][ROT_L] = {{0,0},{1,0},{1,1},{0,-2},{1,-2}},
    [ROT_L][ROT_2] = {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}},

    [ROT_L][ROT_0] = {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}},
    [ROT_0][ROT_L] = {{0,0},{1,0},{1,1},{0,-2},{1,-2}}
};

static const Kick I_KICKS[4][4][5] = {
    [ROT_0][ROT_R] = {{0,0},{-2,0},{1,0},{-2,-1},{1,2}},
    [ROT_R][ROT_0] = {{0,0},{2,0},{-1,0},{2,1},{-1,-2}},

    [ROT_R][ROT_2] = {{0,0},{-1,0},{2,0},{-1,2},{2,-1}},
    [ROT_2][ROT_R] = {{0,0},{1,0},{-2,0},{1,-2},{-2,1}},

    [ROT_2][ROT_L] = {{0,0},{2,0},{-1,0},{2,1},{-1,-2}},
    [ROT_L][ROT_2] = {{0,0},{-2,0},{1,0},{-2,-1},{1,2}},

    [ROT_L][ROT_0] = {{0,0},{1,0},{-2,0},{1,-2},{-2,1}},
    [ROT_0][ROT_L] = {{0,0},{-1,0},{2,0},{-1,2},{2,-1}}
};

int grid_SRS(Grid *g, Tetromino *t, Action a) {
    Tetromino rotated = *t;
    tetromino_rotate(&rotated, a);

    // Direct position
    if (grid_check_position(g, &rotated, rotated.pos) == GRID_OK) {
        *t = rotated;
        return 1;
    }
    
    // Wall kick
    int old_rot = t->rot;
    int new_rot = a == ROTATE_RIGHT ? (old_rot + 1) % 4 : (old_rot + 3) % 4;
    const Kick *kicks = t->type == _I ? I_KICKS[old_rot][new_rot] : JLSTZ_KICKS[old_rot][new_rot];
    
    for (int i = 0; i < 5; i++) {
        Position new_pos = {
            t->pos.x + kicks[i].dx,
            t->pos.y + kicks[i].dy
        };

        if (grid_check_position(g, &rotated, new_pos) == GRID_OK) {
            rotated.pos = new_pos;
            *t = rotated;
            return 1;
        }
    }
    return 0;
}
