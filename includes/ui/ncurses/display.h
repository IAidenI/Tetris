#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncursesw/curses.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include "core/game.h"
#include "ui/ncurses/colors.h"
#include "ui/ncurses/input.h"

#define MIDLESCORE       "─"
#define CORNER_TOP_LEFT  "╭"
#define CORNER_TOP_RIGHT "╮"
#define CORNER_BOT_LEFT  "╰"
#define CORNER_BOT_RIGHT "╯"
#define WALL             "│"
#define CROSS_LEFT       "├"
#define CROSS_RIGHT      "┤"

#define TETROMINO         "[]"
#define TETROMINO_PREVIEW "::"
#define NO_TETROMINO      "  "

#define MENU_ITEMS_START 2
#define MENU_ITEMS_PAUSE 2
#define MENU_ITEMS_LOOSE 1
#define RIGHT_ARROW "▶"
#define LEFT_ARROW  "◀"

void display_init();
void display_render(const Game *g);

#endif // DISPLAY_H
