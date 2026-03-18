#include "core/game.h"

void game_init(Game *g) {
    srand(time(NULL)); // Initialise 7-bag

    grid_init(&g->grid);            // 
    g->current = tetromino_get(__); // Initialize grid
    g->next    = tetromino_get(__); // and needed
    g->preview = tetromino_get(__); // tetromino
    g->hold    = tetromino_get(__); // 

    g->hold_request = 0; // Initialize
    g->has_hold     = 0; // hold settings

    g->level = 1;
    g->score = 0;

    g->lock_delay_start = -1; // lock delay

    g->hard_drop = 0;

    g->status = START; // Set status game to START, not RUNNING
                       // START   : Indicates that the game is ready to by started
                       // RUNNING : Indicates that the game si running (so started)
}

static void game_refresh_preview(Game *g) {
    g->preview = g->current; // Set the preview as the current tetromino
    
    // Iterates the Y preview's position from the current tetromino position
    while (1) {
        Position next = g->preview.pos;
        next.y++;
        
        // Exit if collision (with an other tetromino or OOB) is detected 
        if (grid_check_position(&g->grid, &g->preview, next) != GRID_OK) break;
        g->preview.pos = next;
    }
}

void game_spawn_tetromino(Game *g) {
    // Avoid spawning tetromino if a snapshot is detected
    if (g->status != SNAPSHOT) {
        g->current     = g->next.type == __ ? seven_bag_get_tetromino() : g->next; // Pick 2 tetromino from the 7-bag
        g->next        = seven_bag_get_tetromino();                                // one for the current, one for the next
        g->current.pos = START_SPAWN; // Set the position at the start

        g->preview     = g->current; // Set the preview
        game_refresh_preview(g);     // 
    }

    // Check if game is over
    GridCheck result = grid_check_position(&g->grid, &g->current, g->current.pos);
    g->status = (result == GRID_OK) ? g->status : LOOSE;
}

int game_update(Game *g) {
    // Don't update if pause or loose is detected
    if (g->status == PAUSED || g->status == LOOSE) return 0;

    int changed = 0; // Check for any changes in the update

    // Handle hard drop
    if (g->hard_drop) {
        g->current = g->preview;                    // Lock the tetromino
        grid_lock_tetromino(&g->grid, &g->current); // to the preview position

        g->level  = g->grid.total_lines_cleared / 10;   // Update score
        g->score += SCORE_TABLE[g->grid.lines_cleared]; // and level

        game_spawn_tetromino(g); // Brings up a new
        g->hard_drop = 0;        // tetromino and
        g->has_hold = 0;         // reset settings

        game_refresh_preview(g);
        return 1;
    }

    // Handle hold
    if (g->hold_request) {
        // Do not hold more than 1 time per tetromino
        if (g->has_hold == 1) {
            g->hold_request = 0;
            return 1;
        }

        if (g->hold.type == __) {
            g->hold = tetromino_get(g->current.type); // If no holded tetromino is detected,
            game_spawn_tetromino(g);                  // directly store the current and get another one
        } else {
            Tetromino tmp  = tetromino_get(g->current.type); // If a tetromino is already present
            g->current     = tetromino_get(g->hold.type);    // swap the current with the holded
            g->current.pos = START_SPAWN;                    // and place it at the spawn
            g->hold        = tmp;                            // 
        }

        g->has_hold     = 1; // Update settings
        g->hold_request = 0; //

        game_refresh_preview(g);
        return 1;
    }

    // Detect if a tick has occurred
    if (tick_should_fall(g->level)) {
        // Try to move the tetromino and check if it can be moved
        GridCheck fall_result = grid_try_apply_move(&g->grid, &g->current, tetromino_move_down(&g->current));        
        if (fall_result == GRID_OK) {
            // If it can move
            changed = 1;              // Update settings
            g->lock_delay_start = -1; // 
        } else if (fall_result == GRID_OUT_OF_BOUNDS || fall_result == GRID_COLLISION) {
            // If it can't move
            double now = get_time();

            if (g->lock_delay_start < 0) g->lock_delay_start = now; // Start lock delay
            else if (now - g->lock_delay_start >= LOCK_DELAY) {
                // If lock delay is expired, place the tetromino
                grid_lock_tetromino(&g->grid, &g->current);
                
                g->level  = g->grid.total_lines_cleared / 10;   // Update score
                g->score += SCORE_TABLE[g->grid.lines_cleared]; // and level
                
                game_spawn_tetromino(g);  // Pick another tetromino

                g->lock_delay_start = -1; // 
                g->has_hold = 0;          // Update settings
                changed = 1;              //
            }
        }
    }

    game_refresh_preview(g);
    return changed;
}

const char *game_get_difficulty(int level) {
    if (level >= 0 && level <= 4) return "Easy";
    else if (level >= 5 && level <= 9) return "Medium";
    else if (level >= 10 && level <= 15) return "Hard";
    else if (level >= 16 && level <= 28) return "Expert";
    else if (level >= 29) return "Insane";
    return "Inconnu";
}

void game_reset(Game *g) {
    game_init(g);
    game_spawn_tetromino(g);
}

int game_is_not_over(Game *g) {
    return g->status != QUIT;
}

void game_pause(Game *g) {
    if (g->status == PAUSED) g->status = RUNNING;
    else g->status = PAUSED;
}

void game_quit(Game *g) {
    g->status = QUIT;
}
