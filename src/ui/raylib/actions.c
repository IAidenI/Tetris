#include "ui/raylib/actions.h"

static void resize_window(Position p) {
    SetWindowSize(p.x, p.y);

    int monitor = GetCurrentMonitor();
    int x = (GetMonitorWidth(monitor)  - p.x) / 2;
    int y = (GetMonitorHeight(monitor) - p.y) / 2;

    SetWindowPosition(x, y);
}

void on_start(void *data) {
    GameStatus *status = data;
    *status = RUNNING;
    resize_window((Position){RAYLIB_GAME_WINDOW_WIDTH, RAYLIB_GAME_WINDOW_HEIGHT});
}

void on_import(void *data) {
    printf("[DEBUG] Not implemented\n");
}

void on_main_menu(void *data) {
    Game *game = data;
    game_reset(game);
    resize_window((Position){RAYLIB_START_WINDOW_WIDTH, RAYLIB_START_WINDOW_HEIGHT});
}

void on_resume(void *data) {
    GameStatus *status = data;
    *status = RUNNING;
}

void on_exit(void *data) {
    GameStatus *status = data;
    *status = QUIT;
}
