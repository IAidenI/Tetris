#ifndef COMPONENT_H
#define COMPONENT_H

#include <raylib.h>
#include "utils/types.h"
#include "ui/raylib/colors.h"

#define BUTTON_MENU_PADX 10.0f
#define BUTTON_MENU_PADY 10.0f

typedef struct {
    const Font *font;
    const char *text;
    float font_size;
    float spacing;
    Color color;
} TextStyle;

static inline void DrawTextStyled(const TextStyle label, Position p) {
    Vector2 pos = { (float)p.x, (float)p.y };
    DrawTextEx(*label.font, label.text, pos, label.font_size, label.spacing, label.color);
}

static inline Vector2 MeasureTextStyled(const TextStyle label) {
    return MeasureTextEx(*label.font, label.text, label.font_size, label.spacing);
}

typedef void (*CallBack)(void *data);
typedef struct {
    CallBack cb;
    void *data;
} ButtonAction;

Size button_menu_size(const TextStyle label);
Size button_menu_max_size(const TextStyle *labels, int count);
void button_menu(const TextStyle label, Position p, Size s, Color c, ButtonAction action);

#endif // COMPONENT_H
