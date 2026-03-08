#ifndef TETROMINOS_H
#define TETROMINOS_H

#include <stdio.h>
#include "utils/tools.h"

#define TETROMINO_SIZE 4

typedef enum {
    TETROMINO_TYPE_NONE,
    TETROMINO_TYPE_I,
    TETROMINO_TYPE_J,
    TETROMINO_TYPE_L,
    TETROMINO_TYPE_O,
    TETROMINO_TYPE_S,
    TETROMINO_TYPE_T,
    TETROMINO_TYPE_Z,
    TETROMINO_TYPE_COUNT
} TetrominoType;

#define __ TETROMINO_TYPE_NONE
#define _I TETROMINO_TYPE_I
#define _J TETROMINO_TYPE_J
#define _L TETROMINO_TYPE_L
#define _O TETROMINO_TYPE_O
#define _S TETROMINO_TYPE_S
#define _T TETROMINO_TYPE_T
#define _Z TETROMINO_TYPE_Z

typedef struct {
    int size;
    TetrominoType shape[TETROMINO_SIZE][TETROMINO_SIZE];
    TetrominoType next_shape[TETROMINO_SIZE][TETROMINO_SIZE];
    Position pos;
    Position next_pos;
} Tetromino;

#define MAKE_TETROMINO(s, ...) { .size = (s), .shape = __VA_ARGS__, .next_shape = {{0}}, .pos = {0, 0}, .next_pos = {-1, -1} }

extern const Tetromino TETROMINO_E;
extern const Tetromino TETROMINO_I;
extern const Tetromino TETROMINO_J;
extern const Tetromino TETROMINO_L;
extern const Tetromino TETROMINO_O; // WARNING ISSUE WITH HIM WHEN ROTATE
extern const Tetromino TETROMINO_S;
extern const Tetromino TETROMINO_T;
extern const Tetromino TETROMINO_Z;

extern TetrominoType seven_bag[TETROMINO_TYPE_COUNT];

void tetromino_rotate(Tetromino *t);
void tetromino_move_left(Tetromino *t);
void tetromino_move_right(Tetromino *t);
void tetromino_move_down(Tetromino *t);
Tetromino get_tetromino(TetrominoType t);

#endif // TETROMINOS_H