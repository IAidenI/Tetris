#include "ui/ncurses/colors.h"

ColorList color_get_list() {
    return (ColorList){
        .colors = {
            NONE,
            LIGHT_BLUE, // TETROMINO_I
            DARK_BLUE,  // TETROMINO_J
            ORANGE,     // TETROMINO_L
            YELLOW,     // TETROMINO_O
            GREEN,      // TETROMINO_S
            PURPLE,     // TETROMINO_T
            RED         // TETROMINO_Z
        }
    };
}

void color_init() {
    if (!has_colors()) return;

    start_color();
    use_default_colors();

    if (can_change_color()) {
        start_color();

        // Tetromino
        init_color(RED, RGB_NCURSES(255, 0, 0));
        init_color(ORANGE, RGB_NCURSES(255, 153, 0));
        init_color(YELLOW, RGB_NCURSES(255, 255, 0));
        init_color(GREEN, RGB_NCURSES(51, 255, 51));
        init_color(LIGHT_BLUE, RGB_NCURSES(0, 255, 255));
        init_color(PURPLE, RGB_NCURSES(153, 0, 205));
        init_color(DARK_BLUE, RGB_NCURSES(0, 0, 255));

        // Score
        init_color(SCORE, RGB_NCURSES(255, 250, 240));
        init_color(LEVEL_EASY, RGB_NCURSES(0, 255, 127));
        init_color(LEVEL_MEDIUM, RGB_NCURSES(255, 255, 0));
        init_color(LEVEL_HARD, RGB_NCURSES(255, 165, 0));
        init_color(LEVEL_EXPERT, RGB_NCURSES(255, 69, 0));
        init_color(LEVEL_INSANE, RGB_NCURSES(128, 0, 128));


        // Tetromino
        init_pair(RED, RED, -1);
        init_pair(ORANGE, ORANGE, -1);
        init_pair(YELLOW, YELLOW, -1);
        init_pair(GREEN, GREEN, -1);
        init_pair(LIGHT_BLUE, LIGHT_BLUE, -1);
        init_pair(PURPLE, PURPLE, -1);
        init_pair(DARK_BLUE, DARK_BLUE, -1);

        // Score
        init_pair(SCORE, SCORE, - 1);
        init_pair(LEVEL_EASY, LEVEL_EASY, - 1);
        init_pair(LEVEL_MEDIUM, LEVEL_MEDIUM, - 1);
        init_pair(LEVEL_HARD, LEVEL_HARD, - 1);
        init_pair(LEVEL_EXPERT, LEVEL_EXPERT, - 1);
        init_pair(LEVEL_INSANE, LEVEL_INSANE, - 1);
    
    }
}
