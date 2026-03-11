#include "ui/ncurses/input.h"

void handle_input(Game *g) {
    int ch = getch();

    switch (ch) {
        case 'q': game_quit(g);  break;
        case 'p': game_pause(g); break;
    }

    if (g->status == PAUSED) return;

    switch (ch) {
        case KEY_LEFT:  if (g->status != PAUSED) tetromino_move_left(&g->current); break;
        case KEY_RIGHT: if (g->status != PAUSED)tetromino_move_right(&g->current); break;
        case KEY_UP:    if (g->status != PAUSED)tetromino_rotate(&g->current);     break;
        case KEY_DOWN:  if (g->status != PAUSED)tetromino_move_down(&g->current);  break;
        case ERR:
        default:  break;
    }
}
