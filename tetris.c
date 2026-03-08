#include "game.h"

int main() {

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
