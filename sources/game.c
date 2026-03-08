#include "game.h"

void game_init(Game *g) {
    // Initialise 7-bag
    srand(time(NULL));

    // Initialize grid
    for (int w = 0; w < GRID_WIDTH; w++) {
        for (int h = 0; h < GRID_HEIGHT; h++) {
            g->grid[w][h] = 0;
        }
    }
}

void game_place_tetromino(Game *g, const Tetromino *t, Position p) {
    Position p_tmp = p;
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) {
                g->grid[p_tmp.x][p_tmp.y] = t->shape[h][w];
            }
            p_tmp.x++;
        }
        p_tmp.y++;
        p_tmp.x = p.x;
    }
}

void game_spawn_tetromino(Game *g) {
    Tetromino t = seven_bag_get_tetromino();
    game_place_tetromino(g, &t, START_SPAWN);
}

void game_display_grid(Game *g) {
    printf("Grid :\n");
    for (int h = 0; h < GRID_HEIGHT; h++) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            printf("%d ", g->grid[w][h]);
        }
        printf("\n");
    }
}
