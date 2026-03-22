#include "ui/raylib/display.h"

static void display_mino(Position p, Color c) {
    Rectangle rect = {
        (float)p.x,
        (float)p.y,
        (float)CELL_WIDTH,
        (float)CELL_HEIGHT
    };
    DrawRectangleRec(rect, c);
}

static void display_tetromino_with_color(const Tetromino *t, Position grid_origin, Color c) {
    Position base = {
        grid_origin.x + GRID_LARGE_BORDER + t->pos.x * (CELL_WIDTH + GRID_THIN_BORDER),
        grid_origin.y + GRID_LARGE_BORDER + t->pos.y * (CELL_HEIGHT + GRID_THIN_BORDER)
    };

    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (t->shape[h][w]) {
                Position p = {
                    base.x + w * (CELL_WIDTH + GRID_THIN_BORDER),
                    base.y + h * (CELL_HEIGHT + GRID_THIN_BORDER)
                };
                display_mino(p, c);
            }
        }
    }
}

static void display_tetromino(const Tetromino *t, Position grid_origin) {
    display_tetromino_with_color(t, grid_origin, color_get(t->type));
}

static void display_tetromino_preview(const Tetromino *t, Position grid_origin) {
    display_tetromino_with_color(t, grid_origin, lighten(color_get(t->type), 30.0f, 20.0f));
}

static void display_grid(const Grid *g, Position p) {
    int total_w = 2 * GRID_LARGE_BORDER + GRID_WIDTH * CELL_WIDTH + (GRID_WIDTH - 1) * GRID_THIN_BORDER;
    int total_h = 2 * GRID_LARGE_BORDER + GRID_HEIGHT * CELL_HEIGHT + (GRID_HEIGHT - 1) * GRID_THIN_BORDER;

    int inner_x = p.x + GRID_LARGE_BORDER;
    int inner_y = p.y + GRID_LARGE_BORDER;

    Rectangle outer = {
        (float)p.x, (float)p.y,
        (float)total_w, (float)total_h
    };
    DrawRectangleRec(outer, COLOR_SPLIT);

    for (int h = 0; h < GRID_HEIGHT; h++) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            Position cell_pos = {
                inner_x + w * (CELL_WIDTH + GRID_THIN_BORDER),
                inner_y + h * (CELL_HEIGHT + GRID_THIN_BORDER)
            };
            display_mino(cell_pos, color_get(g->cell[h][w]));
        }
    }
}

static void display_menu_start(const Manager *m) {
    DrawText("Tetris", 190, 200, 20, RED);
    const TextStyle label_new_game = {
        .font = manager_get_font(m, ENTITY_LABEL, 20),
        .text = "New Game",
        .fontSize = 20.0f,
        .spacing = 2.0f,
        .color = BLACK
    };
    button_basic(label_new_game, (Position){190, 250});

    const TextStyle label_exit = {
        .font = manager_get_font(m, ENTITY_LABEL, 20),
        .text = "Exit",
        .fontSize = 20.0f,
        .spacing = 2.0f,
        .color = BLACK
    };
    button_basic(label_exit, (Position){190, 300});
}

void display_render(const Game *g, const Manager *m) {
    log_write("Status : %d\n", g->status);
    /*if (g->status == START) {
        display_menu_start(m);
        return;
    }*/
    
    Position grid_origin = { 20, 20 };

    display_grid(&g->grid, grid_origin);
    display_tetromino(&g->current, grid_origin);
    display_tetromino_preview(&g->preview, grid_origin);
}
