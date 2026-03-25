#ifndef GAME_H
#define GAME_H

#include "core/tetrominos.h"
#include "core/seven_bag.h"
#include "core/grid.h"
#include "core/tick.h"
#include "core/game_settings.h"
#include "info.h"
#include "utils/debug.h"

#define MESSAGE_SIZE 256

typedef struct {
    Grid grid;
    
    Tetromino current;
    Tetromino next;
    Tetromino preview;
    Tetromino hold;
    
    int hold_request; // Ask for hold current tetromino
    int has_hold;     // Check if a request has already been successful

    int level;
    int score;

    double lock_delay_start;

    int hard_drop;

    GameStatus status;

    char message[MESSAGE_SIZE];
    MessageLevel message_level;
    double message_until;
} Game;

void game_init(Game *g);
void game_spawn_tetromino(Game *g);
int  game_update(Game *g);
const char *game_get_difficulty(int level);
void game_reset(Game *g);
int  game_is_not_over(Game *g);
void game_pause(Game *g);
void game_quit(Game *g);
void game_cleanup(Game *g);
void game_set_message(Game *g, const char *text, MessageLevel level, double duration);
void game_clear_message(Game *g);

#endif // GAME_H