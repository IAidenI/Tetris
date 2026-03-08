#include "tetrominos.h"

const Tetromino TETROMINO_E = MAKE_TETROMINO(4,
   {{__,__,__,__},
    {__,__,__,__},
    {__,__,__,__},
    {__,__,__,__}}
);

const Tetromino TETROMINO_I = MAKE_TETROMINO(4,
   {{__,__,__,__},
    {_I,_I,_I,_I},
    {__,__,__,__},
    {__,__,__,__}}
);

const Tetromino TETROMINO_J = MAKE_TETROMINO(3,
   {{_J,__,__},
    {_J,_J,_J},
    {__,__,__}}
);

const Tetromino TETROMINO_L = MAKE_TETROMINO(3,
   {{__,__,_L},
    {_L,_L,_L},
    {__,__,__}}
);

// WARNING ISSUE WITH HIM WHEN ROTATE
const Tetromino TETROMINO_O = MAKE_TETROMINO(3,
   {{__,_O,_O},
    {__,_O,_O},
    {__,__,__}}
);

const Tetromino TETROMINO_S = MAKE_TETROMINO(3,
   {{__,_S,_S},
    {_S,_S,__},
    {__,__,__}}
);

const Tetromino TETROMINO_T = MAKE_TETROMINO(3,
   {{__,_T,__},
    {_T,_T,_T},
    {__,__,__}}
);

const Tetromino TETROMINO_Z = MAKE_TETROMINO(3,
   {{_Z,_Z,__},
    {__,_Z,_Z},
    {__,__,__}}
);

void tetromino_rotate(Tetromino *t) {
    // Temporary store value to not overwrite
    int tmp[TETROMINO_SIZE][TETROMINO_SIZE];
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            tmp[w][t->size - 1 - h] = t->shape[h][w];
        }
    }

    // Place new values
    for (int h = 0; h < t->size; h++) {
        for (int w = 0; w < t->size; w++) {
            t->shape[h][w] = tmp[h][w];
        }
    }
}

Tetromino get_tetromino(TetrominoType t) {
    switch (t) {
        case _I:  return TETROMINO_I;
        case _J:  return TETROMINO_J;
        case _L:  return TETROMINO_L;
        case _O:  return TETROMINO_O;
        case _S:  return TETROMINO_S;
        case _T:  return TETROMINO_T;
        case _Z:  return TETROMINO_Z;
        case __:
        default: return TETROMINO_E;
    }
}

void tetromino_display_infos(const Tetromino *t) {
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("Position : X=%d;Y=%d\n", t->pos.x, t->pos.y);
    printf("Shape    :\n");
    
    for (int h = 0; h < t->size; h++) {
        printf("           ");
        for (int w = 0; w < t->size; w++) {
            printf("%d ", t->shape[h][w]);
        }
        printf("\n");
    }
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
}
