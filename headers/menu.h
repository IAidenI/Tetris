#ifndef MENU_H
#define MENU_H

#include "game.h"
#include "colors.h"
#include "function_ret.h"
#include "ihm.h"

#define MENU_ITEMS 2

int Menu(const int is_loose);
void Help(const char* buffer);

#endif
