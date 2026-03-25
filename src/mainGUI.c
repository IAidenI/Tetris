#include "core/game.h"
#include "ui/raylib/display.h"
#include "ui/raylib/input.h"
#include "utils/snapshot.h"
#include "ui/raylib/manager.h"

int main() {
    InitWindow(RAYLIB_START_WINDOW_WIDTH, RAYLIB_START_WINDOW_HEIGHT, "Tetris");
    SetTargetFPS(60);

    Game game;
    Manager manager;
    game_init(&game);
    if (manager_init(&manager)) return 1;
    log_init("./debug.log");
	snapshot_init("./snapshot.ini");

    SetExitKey(KEY_NULL);
    game_spawn_tetromino(&game);
    while (!WindowShouldClose() && game_is_not_over(&game)) {
        input_handle(&game);
        if (!game_is_not_over(&game)) break;

        game_update(&game);
        if (!game_is_not_over(&game)) break;

        BeginDrawing();
            ClearBackground(COLOR_BACKGROUND);

            display_render(&game, &manager);

        EndDrawing();
    }
    manager_cleanup(&manager);
    game_cleanup(&game);
    CloseWindow();
    return 0;
}
