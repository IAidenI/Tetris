#include "ui/ncurses/display.h"

void display_init() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
    set_escdelay(25);
}

static void display_frame(Position pos, Size size) {
    int y = pos.y;
    int x = pos.x;

    // Top border
    mvprintw(y, x, CORNER_TOP_LEFT);

    for (int w = 0; w < size.w; w++) {
        mvprintw(y, x + 1 + w, MIDLESCORE);
    }

    mvprintw(y, x + size.w + 1, CORNER_TOP_RIGHT);

    // Middle
    for (int h = 0; h < size.h; h++) {
        mvprintw(y + 1 + h, x, WALL);
        for (int w = 0; w < size.w; w++) {
            mvaddch(y + 1 + h, x + 1 + w, ' ');
        }
        mvprintw(y + 1 + h, x + size.w + 1, WALL);
    }

    // Bottom border
    mvprintw(y + size.h + 1, x, CORNER_BOT_LEFT);

    for (int w = 0; w < size.w; w++) {
        mvprintw(y + size.h + 1, x + 1 + w, MIDLESCORE);
    }
    mvprintw(y + size.h + 1, x + size.w + 1, CORNER_BOT_RIGHT);
}

static void display_grid(const Grid *g, Position pos) {
    ColorList color_list = color_get_list();

    for (int h = 0; h < g->height; h++) {
        for (int w = 0; w < g->width; w++) {
            int tetromino = g->cell[h * g->width + w];

            if (tetromino) attron(COLOR_PAIR(color_list.colors[tetromino]));
            mvprintw(pos.y + h, pos.x + (w * 2), tetromino ? TETROMINO : NO_TETROMINO);
            if (tetromino) attroff(COLOR_PAIR(color_list.colors[tetromino]));
        }
    }
}

static void display_tetromino(const Tetromino *t, Position pos, const char *symbol) {
    ColorList color_list = color_get_list();

    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {

            if (!t->shape[h][w])
                continue;

            int x = pos.x + (t->pos.x + w) * 2;
            int y = pos.y + t->pos.y + h;

            attron(COLOR_PAIR(color_list.colors[t->type]));
            mvprintw(y, x, "%s", t->shape[h][w] ? symbol : NO_TETROMINO);
            attroff(COLOR_PAIR(color_list.colors[t->type]));
        }
    }
}

static void display_tetromino_panel_frame(const char *label, Position pos, Size size) {
    int x = pos.x;
    int y = pos.y;

    const int outer_w = size.w;
    const int outer_h = size.h;

    const int left_x   = x;
    const int right_x  = x + outer_w - 1;
    const int top_y    = y;
    const int title_y  = y + 1;
    const int split_y  = y + 2;
    const int content_y = y + 3;
    const int bottom_y = y + outer_h - 1;

    const int inner_w = outer_w - 2;
    const int content_h = outer_h - 4;

    mvprintw(top_y, left_x, "%s", CORNER_TOP_LEFT);
    for (int i = 0; i < inner_w; i++) {
        mvprintw(top_y, left_x + 1 + i, "%s", MIDLESCORE);
    }
    mvprintw(top_y, right_x, "%s", CORNER_TOP_RIGHT);

    mvprintw(title_y, left_x, "%s", WALL);
    mvprintw(title_y, left_x + 1 + (inner_w - (int)strlen(label)) / 2, "%s", label);
    mvprintw(title_y, right_x, "%s", WALL);

    mvprintw(split_y, left_x, "%s", CROSS_LEFT);
    for (int i = 0; i < inner_w; i++) {
        mvprintw(split_y, left_x + 1 + i, "%s", MIDLESCORE);
    }
    mvprintw(split_y, right_x, "%s", CROSS_RIGHT);

    for (int h = 0; h < content_h; h++) {
        int row = content_y + h;

        mvprintw(row, left_x, "%s", WALL);
        for (int w = 0; w < inner_w; w++) {
            mvaddch(row, left_x + 1 + w, ' ');
        }
        mvprintw(row, right_x, "%s", WALL);
    }

    mvprintw(bottom_y, left_x, "%s", CORNER_BOT_LEFT);
    for (int i = 0; i < inner_w; i++) {
        mvprintw(bottom_y, left_x + 1 + i, "%s", MIDLESCORE);
    }
    mvprintw(bottom_y, right_x, "%s", CORNER_BOT_RIGHT);
}

static void display_tetromino_panel(const Tetromino *t, Position pos, Size size) {
    ColorList color_list = color_get_list();

    const int inner_w   = size.w - 2;
    const int content_h = size.h - 4;

    const int content_x = pos.x + 1;
    const int content_y = pos.y + 3;

    Position origin = {
        content_x + (inner_w - t->size * 2) / 2,
        content_y + (content_h - t->size) / 2
    };

    attron(COLOR_PAIR(color_list.colors[t->type]));

    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            if (!t->shape[h][w]) continue;

            int x = origin.x + w * 2;
            int y = origin.y + h;

            mvprintw(y, x, "%s", TETROMINO);
        }
    }

    attroff(COLOR_PAIR(color_list.colors[t->type]));
}

static void display_level(int level, Position pos) {
    int x = pos.x;
    int y = pos.y;

    const char *label = "Niveau : ";
    mvprintw(y, x, "%s", label);
    x += strlen(label);

    const char *difficulty = game_get_difficulty(level);

    int color_pair;
    if (strcmp(difficulty, "Easy") == 0) {
        color_pair = LEVEL_EASY;
    } else if (strcmp(difficulty, "Medium") == 0) {
        color_pair = LEVEL_MEDIUM;
    } else if (strcmp(difficulty, "Hard") == 0) {
        color_pair = LEVEL_HARD;
    } else if (strcmp(difficulty, "Expert") == 0) {
        color_pair = LEVEL_EXPERT;
    } else if (strcmp(difficulty, "Insane") == 0) {
        color_pair = LEVEL_INSANE;
    } else {
        color_pair = 0;
    }

    char level_str[8];
    snprintf(level_str, sizeof(level_str), "%d", level);

    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s - %s", level_str, difficulty);
    attroff(COLOR_PAIR(color_pair));
}

static void display_score(int score, Position pos, Size size) {
    int y = pos.y;

    const char *label = "Score";
    int label_x = pos.x + (size.w - strlen(label)) / 2 + 1;

    mvprintw(y, label_x, "%s", label);

    char score_str[16];
    snprintf(score_str, sizeof(score_str), "%d", score);

    int score_x = pos.x + (size.w - strlen(score_str)) / 2 + 1;

    mvprintw(y + 1, score_x, "%s", score_str);
}

static void display_commands(Position pos) {
    int col2 = pos.x + 22;

    mvaddwstr(pos.y,     pos.x, L"  ↑ : Rotate (CW)");
    mvaddwstr(pos.y + 1, pos.x, L"  ← : Move Left");
    mvaddwstr(pos.y + 2, pos.x, L"  → : Move Right");
    mvaddwstr(pos.y + 3, pos.x, L"  ↓ : Soft Drop");

    mvaddwstr(pos.y,     col2, L"  s : Snapshot");
    mvaddwstr(pos.y + 1, col2, L"  F1 / Esc : Pause");
    mvaddwstr(pos.y + 2, col2, L"  q : Quit");

    mvaddwstr(pos.y + 5, pos.x, L"  Space : Hard Drop");
    mvaddwstr(pos.y + 6, pos.x, L"  c : Hold");
    mvaddwstr(pos.y + 7, pos.x, L"  z : Rotate (CCW)");
}

static int display_get_UTF8_width(const char *str) {
    wchar_t wstr[128];
    mbstowcs(wstr, str, 128);
    return wcswidth(wstr, wcslen(wstr));
}

static void display_menu_start(Position pos, Size size, int selected) {
    const char *menu[MENU_ITEMS_START] = { "New Game", "Exit" };

    int center_x = pos.x + size.w / 2;
    int center_y = pos.y + size.h / 2;

    int title_y = center_y - 2;
    int title_x = center_x - strlen("TETRIS") / 2;

    int menu_spacing_from_title = 2;

    attron(COLOR_PAIR(RED));        mvprintw(title_y, title_x,     "T"); attroff(COLOR_PAIR(RED));
    attron(COLOR_PAIR(ORANGE));     mvprintw(title_y, title_x + 1, "E"); attroff(COLOR_PAIR(ORANGE));
    attron(COLOR_PAIR(YELLOW));     mvprintw(title_y, title_x + 2, "T"); attroff(COLOR_PAIR(YELLOW));
    attron(COLOR_PAIR(GREEN));      mvprintw(title_y, title_x + 3, "R"); attroff(COLOR_PAIR(GREEN));
    attron(COLOR_PAIR(LIGHT_BLUE)); mvprintw(title_y, title_x + 4, "I"); attroff(COLOR_PAIR(LIGHT_BLUE));
    attron(COLOR_PAIR(PURPLE));     mvprintw(title_y, title_x + 5, "S"); attroff(COLOR_PAIR(PURPLE));

    for (int i = 0; i < MENU_ITEMS_START; i++) {
        int y = title_y + menu_spacing_from_title + i;
        
        char buffer[64];
        if (i == selected) snprintf(buffer, sizeof(buffer), "%s %s %s", RIGHT_ARROW, menu[i], LEFT_ARROW);
        else snprintf(buffer, sizeof(buffer), "%s", menu[i]);
        
        int x = center_x - display_get_UTF8_width(buffer) / 2;
        mvprintw(y, x, "%s", buffer);
    }
}

static void display_menu_pause(Position pos, Size size, int selected) {
    display_frame(pos, size);

    const char *menu[MENU_ITEMS_PAUSE] = { "Resume", "Exit" };

    int inner_x = pos.x + 1;
    int inner_y = pos.y + 1;

    int center_x = inner_x + size.w / 2;
    int center_y = inner_y + size.h / 2;

    const char *title = "⏸ Pause";
    int title_y = center_y - 2;
    int title_x = center_x - display_get_UTF8_width(title) / 2;
    mvprintw(title_y, title_x, "%s", title);

    int menu_spacing_from_title = 2;
    for (int i = 0; i < MENU_ITEMS_PAUSE; i++) {
        int y = title_y + menu_spacing_from_title + i;
        
        char buffer[64];
        if (i == selected) snprintf(buffer, sizeof(buffer), "%s %s %s", RIGHT_ARROW, menu[i], LEFT_ARROW);
        else snprintf(buffer, sizeof(buffer), "%s", menu[i]);
        
        int x = center_x - display_get_UTF8_width(buffer) / 2;
        mvprintw(y, x, "%s", buffer);
    }
}

static void display_menu_game_over(Position pos, Size size, int selected) {
    display_frame(pos, size);

    const char *menu[MENU_ITEMS_LOOSE] = { "Main Menu" };

    int inner_x = pos.x + 1;
    int inner_y = pos.y + 1;

    int center_x = inner_x + size.w / 2;
    int center_y = inner_y + size.h / 2;

    const char *title = "Game Over";
    int title_y = center_y - 2;
    int title_x = center_x - display_get_UTF8_width(title) / 2;
    mvprintw(title_y, title_x, "%s", title);

    int menu_spacing_from_title = 2;
    for (int i = 0; i < MENU_ITEMS_LOOSE; i++) {
        int y = title_y + menu_spacing_from_title + i;
        
        char buffer[64];
        if (i == selected) snprintf(buffer, sizeof(buffer), "%s %s %s", RIGHT_ARROW, menu[i], LEFT_ARROW);
        else snprintf(buffer, sizeof(buffer), "%s", menu[i]);
        
        int x = center_x - display_get_UTF8_width(buffer) / 2;
        mvprintw(y, x, "%s", buffer);
    }
}

void display_render(const Game *g) {
    clear();
    log_write("Status : %d\n", g->status);
    Position frame_pos = {0,0};
    Size grid_size = { GRID_WIDTH * 2, GRID_HEIGHT };
    display_frame(frame_pos, grid_size);

    if (g->status == START) {
        display_menu_start(frame_pos, grid_size, input_get_selected_menu());
        refresh();
        return;
    }

    Position grid_origin = {frame_pos.x + 1, frame_pos.y + 1};
    display_grid(&g->grid, grid_origin);
    display_tetromino(&g->preview, grid_origin, TETROMINO_PREVIEW);
    display_tetromino(&g->current, grid_origin, TETROMINO);

    if (g->status == PAUSED) {
        Size pause_size = {PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT};
        Position pause_pos = {
            frame_pos.x + (grid_size.w - pause_size.w) / 2,
            frame_pos.y + (grid_size.h - pause_size.h) / 2
        };
        display_menu_pause(pause_pos, pause_size, input_get_selected_menu());
    } else if (g->status == LOOSE) {
        Size game_over_size = {GAME_OVER_MENU_WIDTH, GAME_OVER_MENU_HEIGHT};
        Position game_over_pos = {
            frame_pos.x + (grid_size.w - game_over_size.w) / 2,
            frame_pos.y + (grid_size.h - game_over_size.h) / 2
        };
        display_menu_game_over(game_over_pos, game_over_size, input_get_selected_menu());
    }

    Position next_pos = {frame_pos.x + grid_size.w + 3, frame_pos.y};
    Size next_size = {14, 6};
    display_tetromino_panel_frame("Next block", next_pos, next_size);
    display_tetromino_panel(&g->next, next_pos, next_size);

    Position hold_pos = {next_pos.x + next_size.w + 3, frame_pos.y};
    Size hold_size = {next_size.w, next_size.h};
    display_tetromino_panel_frame("Hold", hold_pos, hold_size);
    display_tetromino_panel(&g->hold, hold_pos, hold_size);

    Position level_pos = {frame_pos.x, frame_pos.y + grid_size.h + 2};
    display_level(g->level, level_pos);

    Position score_pos = {next_pos.x, next_pos.y + next_size.h + 1};
    display_score(g->score, score_pos, next_size);

    Position command_pos = {score_pos.x, score_pos.y + 5};
    display_commands(command_pos);
    refresh();
}
