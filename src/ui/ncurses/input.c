#include "ui/input.h"

int handle_input(Game *g) {
    int ch = getch();
    switch (ch) {
        case KEY_LEFT:  tetromino_move_left(&g->current);  return 1;
        case KEY_RIGHT: tetromino_move_right(&g->current); return 1;
        case KEY_UP:    tetromino_rotate(&g->current);     return 1;
        case KEY_DOWN:  tetromino_move_down(&g->current);  return 1;
        case 'q':       quit(g);                           return 1;
        case 'p':       pause(g);                          return 1;
        case ERR:
        default:  return 0;
    }
}
