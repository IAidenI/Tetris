#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>

#define RGB_NCURSES(r, g, b) (r * 1000 / 255), (g * 1000 / 255), (b * 1000 / 255)
#define RED 1
#define ORANGE 2
#define YELLOW 3
#define GREEN 4
#define LIGHT_BLUE 5
#define PURPLE 6
#define DARK_BLUE 7

void Init_Colors();

#endif
