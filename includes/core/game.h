#ifndef GAME_H
#define GAME_H

#include "core/tetrominos.h"
#include "core/seven_bag.h"
#include "core/grid.h"
#include "core/tick.h"
#include "core/game_settings.h"
#include "info.h"
#include "utils/debug.h"

typedef enum {
    SNAPSHOT,
    RUNNING,
    PAUSED,
    QUIT,
    OVER
} Status;

typedef struct {
    Grid grid;
    
    Tetromino current;
    Tetromino next;
    Tetromino preview;
    Tetromino hold;
    int hold_request;
    int has_hold;

    int level;
    int score;

    int grounded;
    double lock_delay_start;

    int hard_drop;

    Status status;
} Game;

void game_init(Game *g);
void game_spawn_tetromino(Game *g);
int  game_update(Game *g);
void game_refresh_preview(Game *g);
int  game_is_not_over(Game *g);
int  game_over(Game *g);
void game_pause(Game *g);
void game_quit(Game *g);

#endif // GAME_H