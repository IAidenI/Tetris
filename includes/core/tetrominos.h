#ifndef TETROMINOS_H
#define TETROMINOS_H

#include <stdio.h>
#include <string.h>
#include "utils/types.h"

#define TETROMINO_SIZE 4

typedef enum {
    TETROMINO_TYPE_NONE, // 0
    TETROMINO_TYPE_I,    // 1
    TETROMINO_TYPE_J,    // 2
    TETROMINO_TYPE_L,    // 3
    TETROMINO_TYPE_O,    // 4
    TETROMINO_TYPE_S,    // 5
    TETROMINO_TYPE_T,    // 6
    TETROMINO_TYPE_Z,    // 7
    TETROMINO_TYPE_COUNT // 8
} TetrominoType;

#define __ TETROMINO_TYPE_NONE //
#define _I TETROMINO_TYPE_I    // 
#define _J TETROMINO_TYPE_J    // 
#define _L TETROMINO_TYPE_L    // More visual for
#define _O TETROMINO_TYPE_O    // the declaration
#define _S TETROMINO_TYPE_S    // 
#define _T TETROMINO_TYPE_T    // 
#define _Z TETROMINO_TYPE_Z    // 

typedef enum {
    ROTATE_LEFT,
    ROTATE_RIGHT
} Action;

typedef enum {
    ROT_0,
    ROT_R,
    ROT_2,
    ROT_L
} Rotation;

typedef struct {
    TetrominoType type;
    TetrominoType shape[TETROMINO_SIZE][TETROMINO_SIZE];
    
    int size;
    Position pos;
    Rotation rot;
} Tetromino;

#define MAKE_TETROMINO(t, s, ...) { .type = (t), .shape = __VA_ARGS__, .size = (s), .pos = {0, 0}, .rot = ROT_0 }

extern const Tetromino TETROMINO_E;
extern const Tetromino TETROMINO_I;
extern const Tetromino TETROMINO_J;
extern const Tetromino TETROMINO_L;
extern const Tetromino TETROMINO_O;
extern const Tetromino TETROMINO_S;
extern const Tetromino TETROMINO_T;
extern const Tetromino TETROMINO_Z;

void tetromino_rotate(Tetromino *t, Action a);
Position tetromino_move_left(Tetromino *t);
Position tetromino_move_right(Tetromino *t);
Position tetromino_move_down(Tetromino *t);
Tetromino tetromino_get(TetrominoType t);

#endif // TETROMINOS_H