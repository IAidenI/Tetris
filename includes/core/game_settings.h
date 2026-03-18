#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#define GRID_WIDTH  10
#define GRID_HEIGHT 20
#define PAUSE_MENU_WIDTH  11
#define PAUSE_MENU_HEIGHT 5
#define GAME_OVER_MENU_WIDTH  13
#define GAME_OVER_MENU_HEIGHT 4

#define START_SPAWN ((Position){ .x = 3, .y = 0 })

#define LOCK_DELAY 0.5

extern const int SCORE_TABLE[5];

#endif // GAME_SETTINGS_H