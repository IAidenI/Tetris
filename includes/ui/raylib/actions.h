#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>

#include <raylib.h>
#include "utils/types.h"
#include "core/game.h"

void on_start(void *data);
void on_import(void *data);
void on_main_menu(void *data);
void on_resume(void *data);
void on_exit(void *data);

#endif // ACTIONS_H
