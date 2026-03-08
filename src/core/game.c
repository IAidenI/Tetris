#include "core/game.h"

void game_init(Game *g) {
    srand(time(NULL)); // Initialise 7-bag
    
    grid_init(&g->grid);
    
    g->current = seven_bag_get_tetromino();
    g->next = seven_bag_get_tetromino();
    
    g->level = 1;
    g->score = 0;

    g->status = RUNNING;
}

void game_spawn_tetromino(Game *g) {
    g->current.pos = START_SPAWN;

    // Place tetromino
    for (int h = 0; h < g->current.size; h++) {
        for (int w = 0; w < g->current.size; w++) {
            if (g->current.shape[h][w]) {
                Position pos = { .x = g->current.pos.x + w, .y = g->current.pos.y + h };
                grid_set_cell(&g->grid, pos, g->current.shape[h][w]);
            }
        }
    }
}

int game_update(Game *g) {
    int changed = 0;
    changed |= grid_apply_move(&g->grid, &g->current);
    changed |= grid_apply_rotation(&g->grid, &g->current);
    if (tick_should_fall(g->level)) {
        tetromino_move_down(&g->current);
        changed |= grid_apply_move(&g->grid, &g->current);
    }
    return changed;
}

void paused(Game *g) {
    g->status = PAUSED;
}

void quit(Game *g) {
    g->status = QUIT;
}
