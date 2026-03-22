#ifndef COLORS_H
#define COLORS_H

#include <raylib.h>
#include "core/tetrominos.h"

#define COLOR_SPLIT      CLITERAL(Color){  38,  38,  38, 255 }
#define COLOR_GRID_VOID  CLITERAL(Color){  48,  48,  48, 255 }
#define COLOR_BACKGROUND CLITERAL(Color){  68,  68,  68, 255 }

static inline Color lighten2(Color c, float amount) {
    float t = amount / 100.0f;

    Color out;
    out.r = (unsigned char)(c.r + (255 - c.r) * t);
    out.g = (unsigned char)(c.g + (255 - c.g) * t);
    out.b = (unsigned char)(c.b + (255 - c.b) * t);
    out.a = (unsigned char)(c.a + (255 - c.b) * t);
    return out;
}

static inline Color lighten(Color c, float lighten_percent, float alpha_percent) {
    if (lighten_percent < 0.0f) lighten_percent = 0.0f;
    if (lighten_percent > 100.0f) lighten_percent = 100.0f;

    if (alpha_percent < 0.0f) alpha_percent = 0.0f;
    if (alpha_percent > 100.0f) alpha_percent = 100.0f;

    float t = lighten_percent / 100.0f;

    c.r = (unsigned char)(c.r + (255.0f - c.r) * t);
    c.g = (unsigned char)(c.g + (255.0f - c.g) * t);
    c.b = (unsigned char)(c.b + (255.0f - c.b) * t);
    c.a = (unsigned char)(255.0f * (alpha_percent / 100.0f));

    return c;
}

Color color_from_tetromino(const TetrominoType t);
Color color_from_difficulty(const char *difficulty);

#endif // COLORS_H
