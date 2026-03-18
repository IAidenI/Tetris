#include "core/game.h"
#include "ui/ncurses/display.h"
#include "ui/ncurses/input.h"
#include <signal.h>

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);

    Game game;
    game_init(&game);
    log_init("./input.log");
	snapshot_init("./snapshot.ini");

    // Parse arguments
    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0) {
            // Snapshot
            if (argc < 3) {
                print_error("Il faut spécifier un nom de fichier. (Voir %s --help)\n", argv[0]);
                return 1;
            }

            // Check if file existe
			char *filename = argv[2];
			if (!file_exists(filename)) {
				print_error("Le fichier %s est introuvable.\n", filename);
				return 1;
			}

            // Load snapshot
			snapshot_init(filename);
            if (snapshot_read(&game)) return 1;
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            // Help
            help(argv[0]);
            return 0;
        } else if (strcmp(argv[1], "--version") == 0) {
            // Version
            printf("%s %s - %s\n", GAME, VERSION, VERSION_DATE);
            printf("  - by %s\n", AUTHOR);
            return 0;
        } else {
            // Unknow
            help(argv[0]);
            return 0;
        }
    }

    display_init();
    color_init();

    // Start game
    /*
        There are nothing significative to change here to have another UI
        Only input_handle and display_render si affect by a different UI
    */
    game_spawn_tetromino(&game);
    while (game_is_not_over(&game)) {
        int input_changed = input_handle(&game); // When input is handle it is important to
                                                 // return 1 if an input is detected to update
                                                 // render only if necessary

        int update_changed = game_update(&game);
        if (input_changed || update_changed) display_render(&game);
    }
    endwin();

    return 0;
}
