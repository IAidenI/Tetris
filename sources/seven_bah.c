#include "seven_bag.h"

TetrominoType seven_bag[TETROMINO_TYPE_COUNT];

void seven_bag_fill() {
    for (int i = 0; i < TETROMINO_TYPE_COUNT; i++) seven_bag[i] = i;
    shuffle_array(seven_bag, TETROMINO_TYPE_COUNT, sizeof(seven_bag[0]));
}

int seven_bag_is_empty() {
    for (int i = 0; i < TETROMINO_TYPE_COUNT; i++) {
        if (seven_bag[i]) return 0;
    }
    return 1;
}

Tetromino seven_bag_get_tetromino() {
    if (seven_bag_is_empty()) seven_bag_fill();

    for (int i = 0; i < TETROMINO_TYPE_COUNT; i++) {
        TetrominoType t = seven_bag[i];
        if (t) {
            seven_bag[i] = __; // Remove tetromino
            return get_tetromino(t);
        }
    }
    return TETROMINO_E;
}
