#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncursesw/curses.h>
#include "core/game.h"
#include "ui/ncurses/colors.h"

void display_init();
void display_grid(const Grid *g);
void display_tetromino(const Tetromino *t);
void display_seven_bag();
void display_next_tetromino(const Tetromino *t);
void render(Game *g);

#endif // DISPLAY_H
