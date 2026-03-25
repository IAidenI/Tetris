#include "ui/raylib/component.h"

Size button_menu_size(const TextStyle label) {
    Vector2 label_size = MeasureTextStyled(label);

    return (Size){
        (int)(label_size.x + 2.0f * BUTTON_MENU_PADX),
        (int)(label_size.y + 2.0f * BUTTON_MENU_PADY)
    };
}

Size button_menu_max_size(const TextStyle *labels, int count) {
    Size max = {0, 0};

    for (int i = 0; i < count; i++) {
        Size s = button_menu_size(labels[i]);

        if (s.w > max.w) max.w = s.w;
        if (s.h > max.h) max.h = s.h;
    }

    return max;
}

void button_menu(const TextStyle label, Position p, Size s, Color c, ButtonAction action) {
    Rectangle button = {
        (float)p.x, (float)p.y,
        (float)s.w, (float)s.h
    };

    // Hover
    if (CheckCollisionPointRec(GetMousePosition(), button)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        c = lighten(c, 0.20f);
        // Left clic
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (action.cb) action.cb(action.data);
        }
    }

    // Display
    float roundness = 0.60f, thick = 2.0f;
    int segments = 32;
    DrawRectangleRounded(button, roundness, segments, c);
    DrawRectangleRoundedLinesEx(button, roundness, segments, thick, darken(c, 0.20f));
    
    // Center messge
    Vector2 label_size = MeasureTextStyled(label);
    Position label_pos = {
        (int)(button.x + (button.width  - label_size.x) / 2.0f),
        (int)(button.y + (button.height - label_size.y) / 2.0f)
    };
    DrawTextStyled(label, label_pos);
}
