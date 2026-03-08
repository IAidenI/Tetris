#ifndef TETROMINOS_H
#define TETROMINOS_H

#include <stdio.h>

#define TETROMINO_SIZE 4

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Bounce;

typedef struct {
    int x;
    int y;
} Position;

typedef enum {
    TETROMINO_TYPE_NONE = 0,
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
    const int size;
    TetrominoType shape[TETROMINO_SIZE][TETROMINO_SIZE];
    Position pos;
} Tetromino;

#define MAKE_TETROMINO(s, ...) { .size = (s), .shape = __VA_ARGS__, .pos = {0, 0} }

extern const Tetromino TETROMINO_E;
extern const Tetromino TETROMINO_I;
extern const Tetromino TETROMINO_J;
extern const Tetromino TETROMINO_L;
extern const Tetromino TETROMINO_O;
extern const Tetromino TETROMINO_S;
extern const Tetromino TETROMINO_T;
extern const Tetromino TETROMINO_Z;

extern TetrominoType seven_bag[TETROMINO_TYPE_COUNT];

void tetromino_rotate(Tetromino *t);
Tetromino get_tetromino(TetrominoType t);
void tetromino_display_infos(const Tetromino *t);

#endif // TETROMINOS_H