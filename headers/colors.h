#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>

#define RGB_NCURSES(r, g, b) (r * 1000 / 255), (g * 1000 / 255), (b * 1000 / 255)
#define NONE       0
#define RED        1
#define ORANGE     2
#define YELLOW     3
#define GREEN      4
#define LIGHT_BLUE 5
#define PURPLE     6
#define DARK_BLUE  7
#define SCORE      8

#define LEVEL_EASY   9
#define LEVEL_MEDIUM 10
#define LEVEL_HARD   11
#define LEVEL_EXPERT 12
#define LEVEL_INSANE 13

#define BLOCK_COUNT 8
typedef struct {
    int colors[BLOCK_COUNT];
} ColorList;

ColorList Get_Color_List();
void Init_Colors();

#endif
