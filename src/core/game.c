#include "core/game.h"

void game_init(Game *g) {
    srand(time(NULL)); // Initialise 7-bag
    
    grid_init(&g->grid);
    g->current = tetromino_get(__);
    g->next    = tetromino_get(__);
    
    g->level = 1;
    g->score = 0;

    g->status = RUNNING;
}

void game_spawn_tetromino(Game *g) {
    g->current = g->next.type == __ ? seven_bag_get_tetromino() : g->next;
    log_write("next_type : %d\n", g->next.type);
    g->next = seven_bag_get_tetromino();
    g->current.pos = START_SPAWN;
}

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
