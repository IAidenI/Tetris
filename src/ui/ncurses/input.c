#include "ui/ncurses/input.h"

int handle_input(Game *g) {
    int ch = getch();
    if (ch == ERR) return 0;

    switch (ch) {
        case 's': snapshot_create(g); return 1;
        case 'q': game_quit(g);       return 1;
        case 27 : // Escape
        case KEY_F(1): game_pause(g); return 1;
    }

    if (g->status == PAUSED) return 0;

    switch (ch) {
        // Hard drop
        case ' ': g->hard_drop    = 1; return 1;
        // Hold
        case 'c': g->hold_request = 1; return 1;
        // Move
        case KEY_LEFT:  return grid_apply_move(&g->grid, &g->current, tetromino_move_left(&g->current));
        case KEY_RIGHT: return grid_apply_move(&g->grid, &g->current, tetromino_move_right(&g->current));
        case KEY_DOWN:  return grid_apply_move(&g->grid, &g->current, tetromino_move_down(&g->current));
        // Rotate clockwise
        case 'x':
        case KEY_UP: return grid_SRS(&g->grid, &g->current, ROTATE_RIGHT);
        // Rotate counter-clockwise
        case 'z':    return grid_SRS(&g->grid, &g->current, ROTATE_LEFT);
        default:     return 0;
    }
}
