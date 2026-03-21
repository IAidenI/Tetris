#include "core/game.h"
#include "ui/raylib/display.h"
#include "utils/snapshot.h"

int main() {
    const int screenWidth = 1300;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    Game game;
    game_init(&game);
    log_init("./debug.log");
	snapshot_init("./snapshot.ini");
    if (snapshot_read(&game)) return 1;
    log_write("Current X=%d;Y=%d\n", game.current.pos.x, game.current.pos.y);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(COLOR_BACKGROUND);

            display_grid(&game.grid, (Position){20, 20});
            display_tetromino(&game.current, (Position){20, 20});

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
