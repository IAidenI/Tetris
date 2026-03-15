#include "ui/ncurses/display.h"

void display_init() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
}

void display_grid_frame(Position pos, Size size) {
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

void display_grid(const Grid *g, Position pos) {
    ColorList color_list = color_get_list();

    for (int h = 0; h < GRID_HEIGHT; h++) {
        for (int w = 0; w < GRID_WIDTH; w++) {
            int tetromino = g->cell[h][w];

            if (tetromino) attron(COLOR_PAIR(color_list.colors[tetromino]));
            mvprintw(pos.y + h, pos.x + (w * 2), tetromino ? TETROMINO : NO_TETROMINO);
            if (tetromino) attroff(COLOR_PAIR(color_list.colors[tetromino]));
        }
    }
}

void display_tetromino(const Tetromino *t, Position pos, const char *symbol) {
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

void display_tetromino_panel_frame(const char *label, Position pos, Size size) {
    int x = pos.x;
    int y = pos.y;

    mvprintw(y, x, "%s", CORNER_TOP_LEFT);
    for (int i = 0; i < size.w; i++) {
        mvprintw(y, x + 1 + i, "%s", MIDLESCORE);
    }
    mvprintw(y, x + size.w + 1, "%s", CORNER_TOP_RIGHT);

    mvprintw(y + 1, x, "%s", WALL);
    mvprintw(y + 1, x + 2, "%s", label);
    mvprintw(y + 1, x + size.w + 1, "%s", WALL);

    mvprintw(y + 2, x, "%s", WALL);
    for (int i = 0; i < size.w; i++) {
        mvprintw(y + 2, x + 1 + i, "%s", MIDLESCORE);
    }
    mvprintw(y + 2, x + size.w + 1, "%s", WALL);

    for (int h = 0; h < size.h; h++) {
        mvprintw(y + 3 + h, x, "%s", WALL);
        for (int w = 0; w < size.w; w++) {
            mvaddch(y + 3 + h, x + 1 + w, ' ');
        }
        mvprintw(y + 3 + h, x + size.w + 1, "%s", WALL);
    }

    mvprintw(y + size.h + 3, x, "%s", CORNER_BOT_LEFT);
    for (int i = 0; i < size.w; i++) {
        mvprintw(y + size.h + 3, x + 1 + i, "%s", MIDLESCORE);
    }
    mvprintw(y + size.h + 3, x + size.w + 1, "%s", CORNER_BOT_RIGHT);
}

void display_tetromino_panel(const Tetromino *t, Position pos, Size size) {
    ColorList color_list = color_get_list();

    Position origin = {
        pos.x + 1 + (size.w / 2) - t->size,
        pos.y + 3 + (size.h / 2) - (t->size / 2)
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

void display_level(int level, Position pos) {
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

void display_score(int score, Position pos, Size size) {
    int y = pos.y;

    const char *label = "Score";
    int label_x = pos.x + (size.w - strlen(label)) / 2 + 1;

    mvprintw(y, label_x, "%s", label);

    char score_str[16];
    snprintf(score_str, sizeof(score_str), "%d", score);

    int score_x = pos.x + (size.w - strlen(score_str)) / 2 + 1;

    mvprintw(y + 1, score_x, "%s", score_str);
}

void display_render(Game *g) {
    clear();
    Position frame_pos = {0,0};
    Size grid_size = {GRID_WIDTH * 2, GRID_HEIGHT};
    display_grid_frame(frame_pos, grid_size);

    Position grid_origin = {frame_pos.x + 1, frame_pos.y + 1};
    display_grid(&g->grid, grid_origin);
    display_tetromino(&g->preview, grid_origin, TETROMINO_PREVIEW);
    display_tetromino(&g->current, grid_origin, TETROMINO);

    Position next_pos = {frame_pos.x + grid_size.w + 3, frame_pos.y};
    Size next_size = {12, 2};
    display_tetromino_panel_frame("Next block", next_pos, next_size);
    display_tetromino_panel(&g->next, next_pos, next_size);

    Position hold_pos = {next_pos.x + next_size.w + 3, frame_pos.y};
    Size hold_size = {12, 2};
    display_tetromino_panel_frame("Hold", hold_pos, hold_size);
    display_tetromino_panel(&g->hold, hold_pos, hold_size);

    Position level_pos = {frame_pos.x, frame_pos.y + grid_size.h + 2};
    display_level(g->level, level_pos);

    Position score_pos = {next_pos.x, next_pos.y + next_size.h + 4 + 1};
    display_score(g->score, score_pos, next_size);
    refresh();
}
