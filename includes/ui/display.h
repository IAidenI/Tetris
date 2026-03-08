#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncursesw/curses.h>
#include "core/game.h"

void display_init();
void display_grid(const Grid *g);
void display_tetromino(const Tetromino *t);
#define display_next_tetromino(Tetromino) display_tetromino(Tetromino)
void render(Game *g);

#endif // DISPLAY_H
