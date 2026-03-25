#ifndef MENUS_H
#define MENUS_H

#include "utils/types.h"
#include "ui/raylib/colors.h"
#include "ui/raylib/component.h"
#include "ui/raylib/manager.h"
#include "ui/raylib/actions.h"

void menu_start(const Manager *m, GameStatus *s);
void menu_pause(const Manager *m, Game *g);
void menu_game_over(const Manager *m, Game *g);

#endif // MENUS_H
