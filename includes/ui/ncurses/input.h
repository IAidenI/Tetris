#ifndef INPUT_H
#define INPUT_H

#include <ncursesw/ncurses.h>
#include "core/game.h"
#include "ui/ncurses/display.h"

int input_handle(Game *g);
int input_get_selected_menu();

#endif // INPUT_H
