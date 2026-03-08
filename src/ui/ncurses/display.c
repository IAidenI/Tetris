#include "ui/display.h"

void display_init() {
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
}

void display_grid(const Grid *g) {
    printw("Grid :\n");
    for (int h = 0; h < GRID_HEIGHT; h++) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            printw("%d ", g->cell[w][h]);
        }
        printw("\n");
    }
}

void display_tetromino(const Tetromino *t) {
    printw("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printw("Position : X=%d;Y=%d\n", t->pos.x, t->pos.y);
    printw("Shape    :\n");
    
    for (int h = 0; h < t->size; h++) {
        printw("           ");
        for (int w = 0; w < t->size; w++) {
            printw("%d ", t->shape[h][w]);
        }
        printw("\n");
    }
    printw("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
}

void render(Game *g) {
    clear();
    display_grid(&g->grid);
    display_tetromino(&g->current);
    display_next_tetromino(&g->next);
    refresh();
}
