#include "ui/ncurses/input.h"

static int selected_menu = 0;
static int input_menu_items_count(GameStatus status) {
    switch (status) {
        case START:  return MENU_ITEMS_START;
        case PAUSED: return MENU_ITEMS_PAUSE;
        case LOOSE:  return MENU_ITEMS_LOOSE;
        default:     return 0;
    }
}

static void input_menu_up(GameStatus status) {
    int count = input_menu_items_count(status);
    if (count <= 0) return;
    selected_menu = (selected_menu == 0) ? count - 1 : selected_menu - 1;
}

static void input_menu_down(GameStatus status) {
    int count = input_menu_items_count(status);
    if (count <= 0) return;
    selected_menu = (selected_menu + 1) % count;
}

int input_get_selected_menu() {
    return selected_menu;
}

int input_handle(Game *g) {
    int ch = getch();
    if (ch == ERR) return 0;

    // General commands
    switch (ch) {
        case 's': snapshot_create(g); return 1;
        case 'q': game_quit(g);       return 1;
        case 27 : // Escape
        case KEY_F(1): game_pause(g); return 1;
    }

    // Paused/GameOver commands
    if (g->status != RUNNING && g->status != SNAPSHOT) {
        switch (ch) {
            case KEY_UP:   input_menu_up(g->status);   return 1;
            case KEY_DOWN: input_menu_down(g->status); return 1;
            case '\n':
            case '\r':
            case ' ':
                switch (g->status) {
                    // Start menu
                    case START: {
                        switch (selected_menu) {
                            case 0: // New Game
                                g->status = RUNNING;
                                selected_menu = 0;
                                return 1;
                            case 1: // Exit
                                g->status = QUIT;
                                selected_menu = 0;
                                return 1;
                        }
                        return 1;
                    }
                    // Paused menu
                    case PAUSED: {
                        switch (selected_menu) {
                            case 0: // Resume
                                g->status = RUNNING;
                                selected_menu = 0;
                                return 1;
                            case 1: // Exit
                                game_reset(g);
                                g->status = START;
                                selected_menu = 0;
                                return 1;
                        }
                        return 1;
                    }
                    // GameOver menu
                    case LOOSE:
                        switch (selected_menu) {
                            case 0: // Main Menu
                                game_reset(g);
                                g->status = START;
                                selected_menu = 0;
                                return 1;
                        }
                        return 1;
                    default: return 0;
                }
            default: return 0;
        }
    }

    // Playing commands
    switch (ch) {
        // Hard drop
        case ' ': g->hard_drop    = 1; return 1;
        // Hold
        case 'c': g->hold_request = 1; return 1;
        // Move
        case KEY_LEFT:  return grid_try_apply_move(&g->grid, &g->current, tetromino_move_left(&g->current));
        case KEY_RIGHT: return grid_try_apply_move(&g->grid, &g->current, tetromino_move_right(&g->current));
        case KEY_DOWN:  return grid_try_apply_move(&g->grid, &g->current, tetromino_move_down(&g->current));
        // Rotate clockwise
        case 'x':
        case KEY_UP: return grid_SRS(&g->grid, &g->current, ROTATE_RIGHT);
        // Rotate counter-clockwise
        case 'z':    return grid_SRS(&g->grid, &g->current, ROTATE_LEFT);
        default:     return 0;
    }
}
