#include "ui/raylib/input.h"

int input_handle(Game *g) {
    // General commands
    if (IsKeyPressed(KEY_S)) { snapshot_create(g); return 1; }
    if (IsKeyPressed(KEY_A)) { game_quit(g);       return 1; } // A --> Q in AZERTY
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_F1)) { game_pause(g); return 1; }

    log_write("Status : %d\n", g->status);
    if (g->status != RUNNING && g->status != SNAPSHOT) return 1;

    // Hard drop
    if (IsKeyPressed(KEY_SPACE)) { g->hard_drop = 1; return 1; }
    // Hold
    if (IsKeyPressed(KEY_C)) { g->hold_request  = 1; return 1; }

    // Move
    if (IsKeyPressed(KEY_LEFT))  return grid_try_apply_move(&g->grid, &g->current, tetromino_move_left(&g->current));
    if (IsKeyPressed(KEY_RIGHT)) return grid_try_apply_move(&g->grid, &g->current, tetromino_move_right(&g->current));
    if (IsKeyPressed(KEY_DOWN))  return grid_try_apply_move(&g->grid, &g->current, tetromino_move_down(&g->current));

    // Rotate CW
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_X)) return grid_SRS(&g->grid, &g->current, ROTATE_RIGHT);
    // Rotate CCW
    if (IsKeyPressed(KEY_Z)) return grid_SRS(&g->grid, &g->current, ROTATE_LEFT);

    return 0;
}
