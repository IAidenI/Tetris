#ifndef BLOCK_H
#define BLOCK_H

#include <wchar.h>

#include "structs.h"
#include "print.h"

// Blocs pour la game
#define BLOCK_COUNT 8
#define SPECIAL_BLOCK_SIZE 4
#define BLOCK_SIZE 3

#define BLOCK            L"[]"
#define NO_BLOCK         L"  "
#define CLEAR_NEXT_BLOCK L"            "

extern void *shapes[BLOCK_COUNT];
void **Get_Shapes();
int Get_Block_Size(int block_id);

typedef void (*BlockActionFunc)(APIGame *game, const int posX, const int posY, const int value, const wchar_t *state);
void For_Each_Block(APIGame *game, const int posX, const int posY, const wchar_t *state, BlockActionFunc action);

#endif