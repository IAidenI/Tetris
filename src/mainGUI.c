#include "core/game.h"
#include "ui/raylib/display.h"
#include "ui/raylib/input.h"
#include "utils/snapshot.h"
#include "ui/raylib/manager.h"

int main() {
    InitWindow(RAYLIB_WINDOW_WIDTH, RAYLIB_WINDOW_HEIGHT, "Tetris");
    SetTargetFPS(60);

    Game game;
    Manager manager;
    game_init(&game);
    manager_init(&manager);
    log_init("./debug.log");
	snapshot_init("./snapshot.ini");
    if (snapshot_read(&game)) return 1;

    SetExitKey(KEY_NULL);
    game_spawn_tetromino(&game);
    while (!WindowShouldClose() && game_is_not_over(&game)) {
        input_handle(&game);
        game_update(&game);
        
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
