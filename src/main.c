#include "core/game.h"

int main(int argc, char **argv) {
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

    /*Tetromino piece = TETROMINO_Z;
    tetromino_display_infos(&piece);

    tetromino_rotate(&piece);
    tetromino_display_infos(&piece);
    
    tetromino_rotate(&piece);
    tetromino_display_infos(&piece);
    
    tetromino_rotate(&piece);
    tetromino_display_infos(&piece);*/

    Game game;
    game_init(&game);
    game_display_grid(&game);
    printf("\n\n");
    game_spawn_tetromino(&game);
    game_display_grid(&game);

    return 0;
}
