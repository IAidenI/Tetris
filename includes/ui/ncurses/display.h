#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncursesw/curses.h>
#include <locale.h>
#include <string.h>
#include "core/game.h"
#include "ui/ncurses/colors.h"

#define MIDLESCORE       "─"
#define CORNER_TOP_LEFT  "╭"
#define CORNER_TOP_RIGHT "╮"
#define CORNER_BOT_LEFT  "╰"
#define CORNER_BOT_RIGHT "╯"
#define WALL             "│"

#define TETROMINO         "[]"
#define TETROMINO_PREVIEW "::"
#define NO_TETROMINO      "  "

void display_init();
void display_grid_frame(Position pos, Size size);
void display_grid(const Grid *g, Position pos);
void display_tetromino(const Tetromino *t, Position pos, const char *symbol);
void display_seven_bag();
void display_tetromino_panel_frame(const char *label, Position pos, Size size);
void display_tetromino_panel(const Tetromino *t, Position pos, Size size);
void display_level(int level, Position pos);
void display_score(int score, Position pos, Size size);
void display_render(Game *g);

#endif // DISPLAY_H
