#include "core/game.h"
#include "ui/ncurses/display.h"
#include "ui/ncurses/input.h"
#include <signal.h>

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);

    Game game;
    game_init(&game);
    log_init("./input.log");
    snapshot_init("./snapshot");

    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0) {
            if (argc < 3) {
                print_error("Il faut spécifier un nom de fichier. (Voir %s --help)\n", argv[0]);
                return 1;
            }

            if (snapshot_read(&game)) return 1;
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            help(argv[0]);
            return 0;
        } else if (strcmp(argv[1], "--version") == 0) {
            printf("%s %s - %s\n", GAME, VERSION, VERSION_DATE);
            printf("  - by %s\n", AUTHOR);
            return 0;
        } else {
            help(argv[0]);
            return 0;
        }
    }
    
    display_init();
    color_init();
    game_spawn_tetromino(&game);
    while (game_is_not_over(&game)) {
        handle_input(&game);
        if (game_update(&game)) render(&game);
    }
    endwin();

    if (game_over(&game)) printf("Game Over !\n");
    
    return 0;
}
