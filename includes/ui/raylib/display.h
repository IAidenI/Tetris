#ifndef DISPLAY_H
#define DISPLAY_H

#include <raylib.h>
#include "utils/types.h"
#include "ui/raylib/colors.h"
#include "ui/raylib/component.h"
#include "ui/raylib/manager.h"
#include "core/game.h"

#define CELL_WIDTH  40
#define CELL_HEIGHT 40
#define GRID_THIN_BORDER  2.0f
#define GRID_LARGE_BORDER 4.0f

void display_render(const Game *g, const Manager *m);

#endif // DISPLAY_H