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
    display_tetromino_with_color(t, grid_origin, color_from_tetromino(t->type));
}

static void display_tetromino_preview(const Tetromino *t, Position grid_origin) {
    display_tetromino_with_color(t, grid_origin, lighten(color_from_tetromino(t->type), 30.0f, 20.0f));
}

static void display_grid(const Grid *g, Position p) {
    Size grid_size = display_get_grid_size(g);

    Rectangle outer = {
        (float)p.x, (float)p.y,
        (float)grid_size.w, (float)grid_size.h
    };
    DrawRectangleRec(outer, COLOR_SPLIT);

    int inner_x = p.x + GRID_LARGE_BORDER;
    int inner_y = p.y + GRID_LARGE_BORDER;

    for (int h = 0; h < g->height; h++) {
        for (int w = 0; w < g->width; w++) {
            Position cell_pos = {
                inner_x + w * (CELL_WIDTH + GRID_THIN_BORDER),
                inner_y + h * (CELL_HEIGHT + GRID_THIN_BORDER)
            };
            display_mino(cell_pos, color_from_tetromino(g->cell[h * g->width + w]));
        }
    }
}

static void display_tetromino_widget(const TextStyle label, const Tetromino *t, Position p) {
    DrawTextStyled(label, p);

    Vector2 label_size = MeasureTextStyled(label);
    int padY = 10;
    p.y += label_size.y + padY;

    int cells[t->size * t->size];
    memset(cells, 0, sizeof(cells));

    Grid fake = {0};
    fake.cell   = cells;
    fake.width  = t->size;
    fake.height = t->size;
    display_grid(&fake, p);
    display_tetromino(t, p);
}

static void display_score(TextStyle label, int score, Position p) {
    label.text  = "Score : ";
    DrawTextStyled(label, p);

    Vector2 prefix_size = MeasureTextStyled(label);
    int prefix_gap_X = 30, prefix_gap_Y = 5;
    p.x += prefix_gap_X;
    p.y += prefix_size.y + prefix_gap_Y;

    char value_str[64];
    snprintf(value_str, sizeof(value_str), "%d", score);

    label.text  = value_str;
    DrawTextStyled(label, p);
}

static void display_level(TextStyle label, int level, Position p) {
    const char *difficulty = game_get_difficulty(level);

    label.text  = "Niveau : ";
    DrawTextStyled(label, p);

    Vector2 prefix_size = MeasureTextStyled(label);
    p.x += prefix_size.x;

    char value_str[64];
    snprintf(value_str, sizeof(value_str), "%d - %s", level, difficulty);

    label.text  = value_str;
    label.color = color_from_difficulty(difficulty);
    DrawTextStyled(label, p);
}

/*static void display_menu_start(const Manager *m) {
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
}*/

void display_render(const Game *g, const Manager *m) {
    /*if (g->status == START) {
        display_menu_start(m);
        return;
    }*/

    // Display grid
    Position grid_origin = { 20, 20 };
    Size grid_size = display_get_grid_size(&g->grid);

    display_grid(&g->grid, grid_origin);
    display_tetromino(&g->current, grid_origin);
    display_tetromino_preview(&g->preview, grid_origin);

    // Display next tetromino
    int panel_gap = 40;
    Position next_pos = {
        grid_origin.x + grid_size.w + panel_gap,
        grid_origin.y
    };

    const TextStyle label_next = {
        .font = manager_get_font(m, ENTITY_LABEL, 20),
        .text = "Next",
        .fontSize = 20.0f,
        .spacing = 2.0f,
        .color = BLACK
    };
    display_tetromino_widget(label_next, &g->next, next_pos);

    // Display hold tetromino
    Size next_size = display_tetromino_widget_get_size(label_next, &g->next);

    Position hold_pos = {
        next_pos.x + next_size.w + panel_gap,
        grid_origin.y
    };

    const TextStyle label_hold = {
        .font = manager_get_font(m, ENTITY_LABEL, 20),
        .text = "Hold",
        .fontSize = 20.0f,
        .spacing = 2.0f,
        .color = BLACK
    };
    display_tetromino_widget(label_hold, &g->hold, hold_pos);

    // Display score
    const TextStyle label_score = {
        .font = manager_get_font(m, ENTITY_LABEL, 20),
        .text = "XXX",
        .fontSize = 20.0f,
        .spacing = 2.0f,
        .color = BLACK
    };

    int score_gap_X = 80;
    Position score_pos = {
        grid_origin.x + grid_size.w + score_gap_X,
        (grid_origin.x + grid_size.h) / 2 - MeasureTextStyled(label_score).y
    };

    display_score(label_score, g->score, score_pos);

    // Display level
    int grid_gap = 20;
    Position level_pos = {
        grid_origin.x,
        grid_origin.y + grid_size.h + grid_gap
    };

    const TextStyle label_level = {
        .font = manager_get_font(m, ENTITY_LABEL, 20),
        .text = "XXX",
        .fontSize = 20.0f,
        .spacing = 2.0f,
        .color = BLACK
    };
    display_level(label_level, g->level, level_pos);
}
