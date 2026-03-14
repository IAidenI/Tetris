#include "ui/ncurses/input.h"

void handle_input(Game *g) {
    int ch = getch();

    switch (ch) {
        case 's': snapshot_create(g); break;
        case 'q': game_quit(g);       break;
        case 27 : // Escape
        case 'p': game_pause(g);      break;
    }

    if (g->status == PAUSED) return;

    switch (ch) {
        case ' ':       g->current = g->ghost;             break;
        case KEY_LEFT:  tetromino_move_left(&g->current);  break;
        case KEY_RIGHT: tetromino_move_right(&g->current); break;
        case KEY_UP:    tetromino_rotate(&g->current);     break;
        case KEY_DOWN:  tetromino_move_down(&g->current);  break;
        case ERR:
        default:  break;
    }
}
