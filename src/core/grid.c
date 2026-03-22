#include "core/grid.h"

void grid_init(Grid *g, Size grid_size) {
    g->width  = grid_size.w;
    g->height = grid_size.h;

    g->lines_cleared       = 0; // Initialize
    g->total_lines_cleared = 0; // of settings

    // Initialize the grid
    g->cell = calloc((size_t)grid_size.w * grid_size.h, sizeof(int));
}

int grid_try_apply_move(Grid *g, Tetromino *t, Position new_pos) {
    GridCheck result = grid_check_position(g, t, new_pos);
    if (result == GRID_OK) t->pos = new_pos;
    return result;
}

static void grid_clear_full_lines(Grid *g) {
    int write_h = g->height - 1;

    // Check all lines from the end
    for (int read_h = g->height - 1; read_h >= 0; read_h--) {
        int line_full = 1;

        // Check if line is full
        for (int w = 0; w < g->width; w++) {
            if (g->cell[read_h * g->width + w] == 0) {
                line_full = 0;
                break;
            }
        }

        // If the line is unfull, copy it in the below line
        if (!line_full) {
            for (int w = 0; w < g->width; w++) {
                g->cell[write_h * g->width + w] = g->cell[read_h * g->width + w];
            }
            write_h--;
        }
    }

    // Clear all remaning lines
    for (int h = write_h; h >= 0; h--) {
        for (int w = 0; w < g->width; w++) {
            g->cell[h * g->width + w] = __;
        }
    }

    int cleared = write_h + 1;
    g->lines_cleared        = cleared; // Update to compute
    g->total_lines_cleared += cleared; // score and level
}

void grid_lock_tetromino(Grid *g, Tetromino *t) {
    // Lock tetromino in the grid with at it's position
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) g->cell[(t->pos.y + h) * g->width + (t->pos.x + w)] = t->shape[h][w];
        }
    }

    grid_clear_full_lines(g);
}

GridCheck grid_check_position(Grid *g, Tetromino *t, Position p) {
    // Iterate the current tetromino's position
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) {
                int gx = p.x + w;
                int gy = p.y + h;

                // Check if bounds are found
                if (gx < 0 || gx >= g->width || gy < 0 || gy >= g->height) return GRID_OUT_OF_BOUNDS;

                // Check if collisons are found
                if (g->cell[gy * g->width + gx] != 0) return GRID_COLLISION;
            }
        }
    }
    return GRID_OK;
}

// Tables from https://tetris.wiki/Super_Rotation_System
// For SRS rotation
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

    // Try with the basique rotation
    if (grid_check_position(g, &rotated, rotated.pos) == GRID_OK) {
        *t = rotated;
        return 1;
    }
    
    // If failed try with wall kick
    int old_rot = t->rot;
    int new_rot = a == ROTATE_RIGHT ? (old_rot + 1) % 4 : (old_rot + 3) % 4;                       // Retrieves the value of the array corresponding
    const Kick *kicks = t->type == _I ? I_KICKS[old_rot][new_rot] : JLSTZ_KICKS[old_rot][new_rot]; // to the current rotation of the tetromino
    
    // Try the 5 tests
    for (int i = 0; i < 5; i++) {
        Position new_pos = {
            t->pos.x + kicks[i].dx,
            t->pos.y + kicks[i].dy
        };

        // If a test is valid the accept it
        if (grid_check_position(g, &rotated, new_pos) == GRID_OK) {
            rotated.pos = new_pos;
            *t = rotated;
            return 1;
        }
    }

    // If nothing works rotation is not apply
    return 0;
}

void grid_cleanup(Grid *g) {
    free(g->cell);
    g->cell = NULL;
}
