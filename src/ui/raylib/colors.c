#include "ui/raylib/colors.h"

Color color_get(const TetrominoType t) {
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
