#include "core/game.h"

void game_init(Game *g) {
    srand(time(NULL)); // Initialise 7-bag

    grid_init(&g->grid);
    g->current = tetromino_get(__);
    g->next    = tetromino_get(__);
    g->preview = tetromino_get(__);
    g->hold    = tetromino_get(__);

    g->hold_request = 0;
    g->has_hold     = 0;

    g->level = 1;
    g->score = 0;

    g->grounded = 0;
    g->lock_delay_start = -1;

    g->hard_drop = 0;

    g->status = RUNNING;
}

void game_spawn_tetromino(Game *g) {
    if (g->status != SNAPSHOT) {
        g->current = g->next.type == __ ? seven_bag_get_tetromino() : g->next;
        g->next    = seven_bag_get_tetromino();
        g->preview = g->current;

        g->current.pos      = START_SPAWN;
        game_refresh_preview(g);
    }

    // Check GameOver
    GridCheck result = grid_check_position(&g->grid, &g->current, g->current.pos);
    g->status = (result == GRID_OK) ? RUNNING : OVER;
}

int game_update(Game *g) {
    if (g->status == PAUSED) return 0;

    int changed = 0;

    if (g->hard_drop) {
        g->current = g->preview;
        grid_lock_tetromino(&g->grid, &g->current);

        g->level  = g->grid.total_lines_cleared / 10;
        g->score += SCORE_TABLE[g->grid.lines_cleared];

        game_spawn_tetromino(g);
        g->hard_drop = 0;
        g->has_hold = 0;

        game_refresh_preview(g);
        return 1;
    }

    if (g->hold_request) {
        if (g->has_hold == 1) {
            g->hold_request = 0;
            return 1;
        }

        if (g->hold.type == __) {
            g->hold = tetromino_get(g->current.type);
            game_spawn_tetromino(g);
        } else {
            Tetromino tmp  = tetromino_get(g->current.type);
            g->current     = tetromino_get(g->hold.type);
            g->current.pos = START_SPAWN;
            g->hold        = tmp;
        }
        g->has_hold     = 1;
        g->hold_request = 0;
        game_refresh_preview(g);
        return 1;
    }

    if (tick_should_fall(g->level)) {
        GridCheck fall_result = grid_apply_move(&g->grid, &g->current, tetromino_move_down(&g->current));
        
        if (fall_result == GRID_OK) {
            changed = 1;
            g->lock_delay_start = -1;
        } else if (fall_result == GRID_OUT_OF_BOUNDS || fall_result == GRID_COLLISION) {
            double now = get_time();
            
            if (g->lock_delay_start < 0) g->lock_delay_start = now;
            else if (now - g->lock_delay_start >= LOCK_DELAY) {
                grid_lock_tetromino(&g->grid, &g->current);
                
                g->level  = g->grid.total_lines_cleared / 10;
                g->score += SCORE_TABLE[g->grid.lines_cleared];
                
                game_spawn_tetromino(g);
                g->lock_delay_start = -1;
                g->has_hold = 0;
                changed = 1;
            }
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

const char *game_get_difficulty(int level) {
    if (level >= 0 && level <= 4) {
        return "Easy";
    } else if (level >= 5 && level <= 9) {
        return "Medium";
    } else if (level >= 10 && level <= 15) {
        return "Hard";
    } else if (level >= 16 && level <= 28) {
        return "Expert";
    } else if (level >= 29) {
        return "Insane";
    }

    return "Inconnu";
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
