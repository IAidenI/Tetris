#include "ui/raylib/colors.h"

Color color_from_tetromino(const TetrominoType t) {
    switch (t) {
        case _I: return CLITERAL(Color){   0, 255, 255, 255};
        case _J: return CLITERAL(Color){   0,   0, 255, 255};
        case _L: return CLITERAL(Color){ 255, 153,   0, 255};
        case _O: return CLITERAL(Color){ 255, 255,   0, 255};
        case _S: return CLITERAL(Color){  51, 255,  51, 255};
        case _T: return CLITERAL(Color){ 153,   0, 205, 255};
        case _Z: return CLITERAL(Color){ 255,   0,   0, 255};
        case __: 
        default: return COLOR_GRID_VOID;
    }
}

Color color_from_difficulty(const char *difficulty) {
    if (strcmp(difficulty, "Easy") == 0)        return CLITERAL(Color){   0, 255, 127, 255};
    else if (strcmp(difficulty, "Medium") == 0) return CLITERAL(Color){ 255, 255,   0, 255};
    else if (strcmp(difficulty, "Hard") == 0)   return CLITERAL(Color){ 255, 165,   0, 255};
    else if (strcmp(difficulty, "Expert") == 0) return CLITERAL(Color){ 255,  69,   0, 255};
    else if (strcmp(difficulty, "Insane") == 0) return CLITERAL(Color){ 128,   0, 128, 255};
    else return PURPLE;
}
