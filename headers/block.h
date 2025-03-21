#ifndef BLOCK_H
#define BLOCK_H

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

#endif