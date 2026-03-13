#include "ui/ncurses/display.h"

void display_init() {
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
}

void display_grid(const Grid *g) {
    printw("Grid :\n");
    ColorList color_list = color_get_list();
    for (int h = 0; h < GRID_HEIGHT; h++) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            int tetromino = g->cell[w][h];

            if (tetromino) attron(COLOR_PAIR(color_list.colors[tetromino]));
            printw("%d ", tetromino);
            if (tetromino) attroff(COLOR_PAIR(color_list.colors[tetromino]));
        }
        printw("\n");
    }
}

void display_grid_(const Grid *g) {
    printf("Grid :\n");
    for (int h = 0; h < GRID_HEIGHT; h++) {
        printf("    ");
        for (int w = 0; w < GRID_WIDTH; w++) {
            printf("%d ", g->cell[w][h]);
        }
        printf("\n");
    }
}

void display_tetromino_(const Tetromino *t) {    
    for (int h = 0; h < t->size; h++) {
        printf("    ");
        for (int w = 0; w < t->size; w++) {
            printf("%d ", t->shape[w][h]);
        }
        printf("\n");
    }
}

void display_tetromino(const Tetromino *t) {
    int saved_y, saved_x;
    getyx(stdscr, saved_y, saved_x);

    ColorList color_list = color_get_list();
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {

            if (t->shape[h][w] == 0) continue;

            int x = t->pos.x + w;
            int y = 1 + t->pos.y + h;

            attron(COLOR_PAIR(color_list.colors[t->type]));
            mvprintw(y, x * 2, "%d", t->shape[h][w]);
            attroff(COLOR_PAIR(color_list.colors[t->type]));
        }
    }

    move(saved_y, saved_x);
}

void display_next_tetromino(const Tetromino *t) {
    printw("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printw("Position : X=%d;Y=%d\n", t->pos.x, t->pos.y);
    printw("Shape    :\n");
    
    ColorList color_list = color_get_list();
    for (int h = 0; h < t->size; h++) {
        printw("           ");
        for (int w = 0; w < t->size; w++) {
            int cell = t->shape[w][h];

            if (cell) attron(COLOR_PAIR(color_list.colors[cell]));
            printw("%d ", cell);
            if (cell) attroff(COLOR_PAIR(color_list.colors[cell]));
        }
        printw("\n");
    }
    printw("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
}

void display_player_infos(int score, int level) {
    printw("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printw("Score : %d\n", score);
    printw("Level : %d\n", level);
    printw("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
}

void render(Game *g) {
    clear();
    display_grid(&g->grid);
    display_tetromino(&g->current);

    //display_next_tetromino(&g->current);
    display_next_tetromino(&g->next);
    display_player_infos(g->score, g->level);
    refresh();
}
