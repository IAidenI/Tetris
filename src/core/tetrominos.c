#include "core/tetrominos.h"

const Tetromino TETROMINO_E = MAKE_TETROMINO(TETROMINO_TYPE_NONE, 4,
   {{__,__,__,__},
    {__,__,__,__},
    {__,__,__,__},
    {__,__,__,__}}
);

const Tetromino TETROMINO_I = MAKE_TETROMINO(TETROMINO_TYPE_I, 4,
   {{__,__,__,__},
    {_I,_I,_I,_I},
    {__,__,__,__},
    {__,__,__,__}}
);

const Tetromino TETROMINO_J = MAKE_TETROMINO(TETROMINO_TYPE_J, 3,
   {{_J,__,__},
    {_J,_J,_J},
    {__,__,__}}
);

const Tetromino TETROMINO_L = MAKE_TETROMINO(TETROMINO_TYPE_L, 3,
   {{__,__,_L},
    {_L,_L,_L},
    {__,__,__}}
);

// WARNING ISSUE WITH HIM WHEN ROTATE
const Tetromino TETROMINO_O = MAKE_TETROMINO(TETROMINO_TYPE_O, 3,
   {{__,_O,_O},
    {__,_O,_O},
    {__,__,__}}
);

const Tetromino TETROMINO_S = MAKE_TETROMINO(TETROMINO_TYPE_S, 3,
   {{__,_S,_S},
    {_S,_S,__},
    {__,__,__}}
);

const Tetromino TETROMINO_T = MAKE_TETROMINO(TETROMINO_TYPE_T, 3,
   {{__,_T,__},
    {_T,_T,_T},
    {__,__,__}}
);

const Tetromino TETROMINO_Z = MAKE_TETROMINO(TETROMINO_TYPE_Z, 3,
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
    memcpy(t->next_shape, tmp, sizeof(tmp));
    t->next_pos = t->pos;
}

void tetromino_move_left(Tetromino *t) {
    t->next_pos = t->pos;
    t->next_pos.x--;
}

void tetromino_move_right(Tetromino *t) {
    t->next_pos = t->pos;
    t->next_pos.x++;
}

void tetromino_move_down(Tetromino *t) {
    t->next_pos = t->pos;
    t->next_pos.y++;
}

Tetromino tetromino_get(TetrominoType t) {
    switch (t) {
        case _I: return TETROMINO_I;
        case _J: return TETROMINO_J;
        case _L: return TETROMINO_L;
        case _O: return TETROMINO_O;
        case _S: return TETROMINO_S;
        case _T: return TETROMINO_T;
        case _Z: return TETROMINO_Z;
        case __:
        default: return TETROMINO_E;
    }
}
