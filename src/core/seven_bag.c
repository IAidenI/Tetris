#include "core/seven_bag.h"

static TetrominoType seven_bag[TETROMINO_TYPE_COUNT - 1];

void seven_bag_fill() {
    for (int i = 0; i < TETROMINO_TYPE_COUNT - 1; i++) seven_bag[i] = i + 1;
    shuffle_array(seven_bag, TETROMINO_TYPE_COUNT - 1, sizeof(seven_bag[0]));
}

int seven_bag_is_empty() {
    for (int i = 0; i < TETROMINO_TYPE_COUNT - 1; i++) {
        if (seven_bag[i]) return 0;
    }
    return 1;
}

Tetromino seven_bag_get_tetromino() {
    if (seven_bag_is_empty()) seven_bag_fill();

    for (int i = 0; i < TETROMINO_TYPE_COUNT - 1; i++) {
        TetrominoType t = seven_bag[i];
        if (t) {
            seven_bag[i] = __; // Remove tetromino
            return tetromino_get(t);
        }
    }
    return tetromino_get(__);
}
