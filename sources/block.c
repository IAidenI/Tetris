#include "block.h"

// On crée les blocs
int BLOCK_RESET[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_I[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_J[BLOCK_SIZE][BLOCK_SIZE] = {
    {2, 0, 0, 0},
    {2, 2, 2, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_L[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 0, 3, 0},
    {3, 3, 3, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_O[BLOCK_SIZE][BLOCK_SIZE] = {
    {4, 4, 0, 0},
    {4, 4, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_S[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 5, 5, 0},
    {5, 5, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_T[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 6, 0, 0},
    {6, 6, 6, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_Z[BLOCK_SIZE][BLOCK_SIZE] = {
    {7, 7, 0, 0},
    {0, 7, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int (*shapes[BLOCK_COUNT])[BLOCK_SIZE] = {
    BLOCK_I,
    BLOCK_J,
    BLOCK_L,
    BLOCK_O,
    BLOCK_S,
    BLOCK_T,
    BLOCK_Z
};

int (**Get_Shapes())[BLOCK_SIZE] {
    return shapes;
}
