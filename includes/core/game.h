#ifndef GAME_H
#define GAME_H

#include "core/tetrominos.h"
#include "core/seven_bag.h"
#include "core/game_settings.h"
#include "info.h"

typedef struct {
    int grid[GRID_WIDTH][GRID_HEIGHT];
    Tetromino current;
    Tetromino next;
} Game;

void game_init(Game *g);
void game_place_tetromino(Game *g, const Tetromino *t, Position p);
void game_spawn_tetromino(Game *g);
void game_display_grid(Game *g);

#endif // GAME_H