#ifndef COLORS_H
#define COLORS_H

#include <raylib.h>
#include "core/tetrominos.h"

#define COLOR_SPLIT      CLITERAL(Color){  38,  38,  38, 255 }
#define COLOR_GRID_VOID  CLITERAL(Color){  48,  48,  48, 255 }
#define COLOR_BACKGROUND CLITERAL(Color){  68,  68,  68, 255 }

Color color_get(const TetrominoType t);

#endif // COLORS_H
