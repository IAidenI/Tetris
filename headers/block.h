// Blocs pour la game
#define BLOCK_COUNT 8
#define BLOCK_SIZE 4

#define BLOCK            L"[]"
#define NO_BLOCK         L"  "
#define CLEAR_NEXT_BLOCK L"            "

extern int (*shapes[BLOCK_COUNT])[BLOCK_SIZE];
int (**Get_Shapes())[BLOCK_SIZE];