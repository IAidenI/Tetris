#include "core/game.h"

void game_init(Game *g) {
    srand(time(NULL)); // Initialise 7-bag

    grid_init(&g->grid);
    g->current = tetromino_get(__);
    g->next    = tetromino_get(__);
    g->preview = tetromino_get(__);

    g->level = 1;
    g->score = 0;

    g->status = RUNNING;
}

void game_spawn_tetromino(Game *g) {
    if (g->status != SNAPSHOT) {
        g->current = g->next.type == __ ? seven_bag_get_tetromino() : g->next;
        g->next    = seven_bag_get_tetromino();
        g->preview = g->current;

        g->current.pos      = START_SPAWN;
        g->current.next_pos = START_SPAWN;
        game_refresh_preview(g);
    }

    // Check GameOver
    GridCheck result = grid_check_position(&g->grid, &g->current, g->current.next_pos);

    if (result == GRID_COLLISION) g->status = OVER;
    else {
        g->current.next_pos = RESET_POSITION;
        g->status = RUNNING;
    }
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

        if (fall_result == GRID_OK) changed = 1;
        else if (fall_result == GRID_OUT_OF_BOUNDS || fall_result == GRID_COLLISION) {
            grid_lock_tetromino(&g->grid, &g->current);

            // Update level and score
            g->level  = g->grid.total_lines_cleared / 10;
            g->score += SCORE_TABLE[g->grid.lines_cleared];

            game_spawn_tetromino(g);
            changed = 1;
        }
    }

    game_refresh_preview(g);
    return changed;
}

void game_refresh_preview(Game *g) {
    g->preview = g->current;
    while (1) {
        Position next = g->preview.pos;
        next.y++;
        
        if (grid_check_position(&g->grid, &g->preview, next) != GRID_OK) break;
        g->preview.pos = next;
    }
}

int game_is_not_over(Game *g) {
    return g->status != QUIT && g->status != OVER;
}

int game_over(Game *g) {
    return g->status == OVER;
}

void game_pause(Game *g) {
    if (g->status == PAUSED) g->status = RUNNING;
    else g->status = PAUSED;
}

void game_quit(Game *g) {
    g->status = QUIT;
}
