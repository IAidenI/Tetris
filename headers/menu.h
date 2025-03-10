#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>

#include "game.h"
#include "colors.h"
#include "function_ret.h"

#define MENU_ITEMS 2

int Menu(const int is_loose);

#endif
