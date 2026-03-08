#ifndef GAME_H
#define GAME_H

#include "core/tetrominos.h"
#include "core/seven_bag.h"
#include "core/grid.h"
#include "core/tick.h"
#include "core/game_settings.h"
#include "info.h"

typedef enum {
    RUNNING,
    PAUSED,
    QUIT
} Status;

typedef struct {
    Grid grid;
    
    Tetromino current;
    Tetromino next;

    int level;
    int score;

    Status status;
} Game;

void game_init(Game *g);
void game_spawn_tetromino(Game *g);
int game_update(Game *g);
void pause(Game *g);
void quit(Game *g);

#endif // GAME_H