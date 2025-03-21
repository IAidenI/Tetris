#include "../headers/block.h"

// On crée les blocs
int BLOCK_RESET[SPECIAL_BLOCK_SIZE][SPECIAL_BLOCK_SIZE] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_I[SPECIAL_BLOCK_SIZE][SPECIAL_BLOCK_SIZE] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int BLOCK_J[BLOCK_SIZE][BLOCK_SIZE] = {
    {2, 0, 0},
    {2, 2, 2},
    {0, 0, 0}
};

int BLOCK_L[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 0, 3},
    {3, 3, 3},
    {0, 0, 0}
};

int BLOCK_O[BLOCK_SIZE][BLOCK_SIZE] = {
    {4, 4, 0},
    {4, 4, 0},
    {0, 0, 0}
};

int BLOCK_S[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 5, 5},
    {5, 5, 0},
    {0, 0, 0}
};

int BLOCK_T[BLOCK_SIZE][BLOCK_SIZE] = {
    {0, 6, 0},
    {6, 6, 6},
    {0, 0, 0}
};

int BLOCK_Z[BLOCK_SIZE][BLOCK_SIZE] = {
    {7, 7, 0},
    {0, 7, 7},
    {0, 0, 0}
};

void *shapes[BLOCK_COUNT] = {
    BLOCK_RESET,
    BLOCK_I,
    BLOCK_J,
    BLOCK_L,
    BLOCK_O,
    BLOCK_S,
    BLOCK_T,
    BLOCK_Z
};

// Tableau pour stocker la taille des blocs
int block_sizes[BLOCK_COUNT] = {
    SPECIAL_BLOCK_SIZE,  // RESET
    SPECIAL_BLOCK_SIZE,  // I
    BLOCK_SIZE,  // O
    BLOCK_SIZE,  // J
    BLOCK_SIZE,  // L
    BLOCK_SIZE,  // S
    BLOCK_SIZE,  // T
    BLOCK_SIZE   // Z
};

void **Get_Shapes() {
    return shapes;
}

int Get_Block_Size(int block_id) {
    return block_sizes[block_id];
}