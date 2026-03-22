#include "ui/raylib/component.h"

void button_basic(const TextStyle label, Position p) {
    Vector2 label_size = MeasureTextStyled(label);

    float padX = 10.0f, padY = 10.0f;
    Rectangle rect = {
        (float)p.x, (float)p.y,
        label_size.x + 2 * padX, label_size.y + 2 * padY
    };
    float roundness = 0.12f, thick = 2.0f;
    int segments = 32;
    DrawRectangleRounded(rect, roundness, segments, BLUE);
    DrawRectangleRoundedLinesEx(rect, roundness, segments, thick, DARKBLUE);
    DrawTextStyled(label, (Position){(float)p.x + padX, (float)p.y + padY});
}
