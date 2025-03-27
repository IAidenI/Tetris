#ifndef MENU_H
#define MENU_H

#define _XOPEN_SOURCE_EXTENDED 1
#include <ncurses.h>

#include "game.h"
#include "colors.h"
#include "function_ret.h"
#include "print.h"
#include "ihm.h"

#define MENU_ITEMS 2

// Dimensions menu pause
#define MENU_PAUSE_HEIGHT 6
#define MENU_PAUSE_WIDTH 12

#define ARROW_LEFT  L"\u25B6"
#define ARROW_RIGHT L"\u25C0"

int Menu_Pause();
int Menu_Confirm();
int Menu(const int is_loose);
void Help(const char* buffer);

#endif
