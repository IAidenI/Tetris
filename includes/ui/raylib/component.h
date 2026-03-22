#ifndef COMPONENT_H
#define COMPONENT_H

#include <raylib.h>
#include "utils/types.h"

typedef struct {
    const Font *font;
    const char *text;
    float fontSize;
    float spacing;
    Color color;
} TextStyle;

static inline void DrawTextStyled(const TextStyle label, Position p) {
    Vector2 pos = { (float)p.x, (float)p.y };
    DrawTextEx(*label.font, label.text, pos, label.fontSize, label.spacing, label.color);
}

static inline Vector2 MeasureTextStyled(const TextStyle label) {
    return MeasureTextEx(*label.font, label.text, label.fontSize, label.spacing);
}

void button_basic(const TextStyle label, Position p);

#endif // COMPONENT_H
