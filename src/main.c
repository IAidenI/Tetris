#include "core/game.h"
#include "ui/display.h"
#include "ui/input.h"
#include <signal.h>

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);

    if (argc > 1) {
        /*if (strcmp(argv[1], "-d") == 0) {
            if (argc < 3) {
                Error("Il faut spécifier un nom de fichier. (Voir %s --help)\n", argv[0]);
                return 1;
            }
        } else*/
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
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

    Game game;
    game_init(&game);
    display_init();

    game_spawn_tetromino(&game);
    while (game.status != QUIT) {
        handle_input(&game);
        if (game_update(&game)) render(&game);
    }

    endwin();
    return 0;
}
