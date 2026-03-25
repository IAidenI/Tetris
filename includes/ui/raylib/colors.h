#ifndef COLORS_H
#define COLORS_H

#include <raylib.h>
#include "core/tetrominos.h"

#define COLOR_SPLIT      CLITERAL(Color){  38,  38,  38, 255 }
#define COLOR_GRID_VOID  CLITERAL(Color){  48,  48,  48, 255 }
#define COLOR_BACKGROUND CLITERAL(Color){  68,  68,  68, 255 }

#define COLOR_MENU             CLITERAL(Color){  68,  68,  68, 255 }
#define COLOR_MENU_TITLE       CLITERAL(Color){ 255, 255,  255,  255 }
#define COLOR_MENU_BUTTON      CLITERAL(Color){  61,  61,  61,  255 }
#define COLOR_MENU_BUTTON_TEXT CLITERAL(Color){ 255, 255,  255,  255 }

#define COLOR_GAME_TEXT CLITERAL(Color){ 255, 255,  255,  255 }

#define COLOR_MESSAGE_BG       CLITERAL(Color){  20,  20,  20, 150 }
#define COLOR_MESSAGE_INFO     CLITERAL(Color){ 220, 220, 220, 220 }
#define COLOR_MESSAGE_WARNING  CLITERAL(Color){ 255, 220, 120, 220 }
#define COLOR_MESSAGE_ERROR    CLITERAL(Color){ 255, 120, 120, 220 }
#define COLOR_MESSAGE_SUCCESS  CLITERAL(Color){ 120, 220, 140, 220 }
#define COLOR_MESSAGE_DEBUG    CLITERAL(Color){ 190, 160, 255, 220 }

static inline Color lighten(Color c, float amount) {
    Color out;
    out.r = c.r + (255 - c.r) * amount;
    out.g = c.g + (255 - c.g) * amount;
    out.b = c.b + (255 - c.b) * amount;
    out.a = c.a;
    return out;
}

static inline Color darken(Color c, float amount) {
    Color out;
    out.r = c.r * (1.0f - amount);
    out.g = c.g * (1.0f - amount);
    out.b = c.b * (1.0f - amount);
    out.a = c.a;
    return out;
}

static inline Color lighten_with_alpha(Color c, float lighten_percent, float alpha_percent) {
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
