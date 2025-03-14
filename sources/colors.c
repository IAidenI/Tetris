#include "../headers/colors.h"

ColorList Get_Color_List() {
    return (ColorList){
        .colors = {
            LIGHT_BLUE, // BLOCK_I
            DARK_BLUE,  // BLOCK_J
            ORANGE,     // BLOCK_L
            YELLOW,     // BLOCK_O
            GREEN,      // BLOCK_S
            PURPLE,     // BLOCK_T
            RED         // BLOCK_Z
        }
    };
}

void Init_Colors() {
    if (can_change_color()) {
        // Activer les couleurs
        start_color();
        init_color(RED, RGB_NCURSES(255, 0, 0));
        init_color(ORANGE, RGB_NCURSES(255, 153, 0));
        init_color(YELLOW, RGB_NCURSES(255, 255, 0));
        init_color(GREEN, RGB_NCURSES(51, 255, 51));
        init_color(LIGHT_BLUE, RGB_NCURSES(0, 255, 255));
        init_color(PURPLE, RGB_NCURSES(153, 0, 205));
        init_color(DARK_BLUE, RGB_NCURSES(0, 0, 255));

        init_pair(RED, RED, -1);
        init_pair(ORANGE, ORANGE, -1);
        init_pair(YELLOW, YELLOW, -1);
        init_pair(GREEN, GREEN, -1);
        init_pair(LIGHT_BLUE, LIGHT_BLUE, -1);
        init_pair(PURPLE, PURPLE, -1);
        init_pair(DARK_BLUE, DARK_BLUE, -1);

    } // Else à faire
}
