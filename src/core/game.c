#include "core/game.h"

void game_init(Game *g) {
    srand(time(NULL)); // Initialise 7-bag
    
    grid_init(&g->grid);
    
    g->level = 1;
    g->score = 0;

    g->status = RUNNING;
}

void game_spawn_tetromino(Game *g) {
    g->current = seven_bag_get_tetromino();
    g->next = seven_bag_get_tetromino();
    g->current.pos = START_SPAWN;

    // Place tetromino
    /*for (int h = 0; h < g->current.size; h++) {
        for (int w = 0; w < g->current.size; w++) {
            if (g->current.shape[h][w]) {
                Position pos = { .x = g->current.pos.x + w, .y = g->current.pos.y + h };
                grid_set_cell(&g->grid, pos, g->current.shape[h][w]);
            }
        }
    }*/
}

/*int game_update(Game *g) {
    if (g->status == PAUSED) return 0;

    int changed = 0;
    changed |= grid_apply_move(&g->grid, &g->current);
    changed |= grid_apply_rotation(&g->grid, &g->current);
    if (tick_should_fall(g->level)) {
        tetromino_move_down(&g->current);
        changed |= grid_apply_move(&g->grid, &g->current);
    }
    return changed;
}*/
int game_update(Game *g) {
    if (g->status == PAUSED) return 0;

    int changed = 0;
    GridCheck move_result = grid_apply_move(&g->grid, &g->current);
    if (move_result == GRID_OK) changed = 1;

    changed |= grid_apply_rotation(&g->grid, &g->current);

    if (tick_should_fall(g->level)) {
        tetromino_move_down(&g->current);

        GridCheck fall_result = grid_apply_move(&g->grid, &g->current);

        if (fall_result == GRID_OK) {
            changed = 1;
        } else if (fall_result == GRID_OUT_OF_BOUNDS || fall_result == GRID_COLLISION) {
            grid_lock_tetromino(&g->grid, &g->current);
            game_spawn_tetromino(g);
            changed = 1;
        }
    }

    return changed;
}

void game_pause(Game *g) {
    if (g->status == PAUSED) g->status = RUNNING;
    else g->status = PAUSED;
}

void game_quit(Game *g) {
    g->status = QUIT;
}
