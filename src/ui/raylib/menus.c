#include "ui/raylib/menus.h"

void menu_start(const Manager *m, GameStatus *s) {
    const TextStyle title = {
        .font = manager_get_font(m, FONT_TITLE, 40),
        .text = "TETRIS",
        .font_size = 40.0f,
        .spacing = 2.0f,
        .color = COLOR_MENU_TITLE
    };

    const TextStyle labels[] = {
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "New Game",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        },
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Import",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        },
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Exit",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        }
    };

    const int label_count = sizeof(labels) / sizeof(labels[0]);

    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int title_w = (int)MeasureTextStyled(title).x;

    Size max_button_size = button_menu_max_size(labels, label_count);

    int line_spacing = 20;
    int total_h = title.font_size + line_spacing + label_count * max_button_size.h + (label_count - 1) * line_spacing;

    int start_y = (screen_h - total_h) / 2;

    int title_x = (screen_w - title_w) / 2;
    int title_y = start_y;

    Position button_pos[] = {
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing
        },
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing + max_button_size.h + line_spacing
        },
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing + 2 * (max_button_size.h + line_spacing)
        }
    };

    DrawTextStyled(title, (Position){title_x, title_y});
    
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    button_menu(labels[0], button_pos[0], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_start, s});
    button_menu(labels[1], button_pos[1], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_import, s});
    button_menu(labels[2], button_pos[2], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_exit, s});
}

void menu_pause(const Manager *m, Game *g) {
    const TextStyle title = {
        .font = manager_get_font(m, FONT_TITLE, 40),
        .text = "Pause",
        .font_size = 40.0f,
        .spacing = 2.0f,
        .color = COLOR_MENU_TITLE
    };

    const TextStyle labels[] = {
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Resume",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        },
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Main Menu",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        },
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Exit",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        }
    };

    const int label_count = sizeof(labels) / sizeof(labels[0]);

    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int title_w = (int)MeasureTextStyled(title).x;

    Size max_button_size = button_menu_max_size(labels, label_count);

    int line_spacing = 20;
    int total_h = title.font_size + line_spacing + label_count * max_button_size.h + (label_count - 1) * line_spacing;

    int start_y = (screen_h - total_h) / 2;

    int title_x = (screen_w - title_w) / 2;
    int title_y = start_y;

    Position button_pos[] = {
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing
        },
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing + max_button_size.h + line_spacing
        },
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing + 2 * (max_button_size.h + line_spacing)
        }
    };

    Size title_size = {
        (int)MeasureTextStyled(title).x,
        (int)MeasureTextStyled(title).y
    };

    int frame_pad_x = 30;
    int frame_pad_y = 25;

    int content_w = (title_size.w > max_button_size.w) ? title_size.w : max_button_size.w;
    int content_h = title_size.h + line_spacing + label_count * max_button_size.h + (label_count - 1) * line_spacing;

    Rectangle frame = {
        (float)((screen_w - (content_w + 2 * frame_pad_x)) / 2),
        (float)((screen_h - (content_h + 2 * frame_pad_y)) / 2),
        (float)(content_w + 2 * frame_pad_x),
        (float)(content_h + 2 * frame_pad_y)
    };
    float roundness = 0.20f, thick = 2.0f;
    int segments = 32;
    DrawRectangleRounded(frame, roundness, segments, COLOR_MENU);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, thick, darken(COLOR_MENU, 0.20f));

    DrawTextStyled(title, (Position){title_x, title_y});
    
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    button_menu(labels[0], button_pos[0], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_resume, &g->status});
    button_menu(labels[1], button_pos[1], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_main_menu, g});
    button_menu(labels[2], button_pos[2], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_exit, &g->status});
}

void menu_game_over(const Manager *m, Game *g) {
    const TextStyle title = {
        .font = manager_get_font(m, FONT_TITLE, 40),
        .text = "Game Over",
        .font_size = 40.0f,
        .spacing = 2.0f,
        .color = COLOR_MENU_TITLE
    };

    const TextStyle labels[] = {
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Main Menu",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        },
        {
            .font = manager_get_font(m, FONT_MENU, 20),
            .text = "Exit",
            .font_size = 20.0f,
            .spacing = 2.0f,
            .color = COLOR_MENU_BUTTON_TEXT
        }
    };

    const int label_count = sizeof(labels) / sizeof(labels[0]);

    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int title_w = (int)MeasureTextStyled(title).x;

    Size max_button_size = button_menu_max_size(labels, label_count);

    int line_spacing = 20;
    int total_h = title.font_size + line_spacing + label_count * max_button_size.h + (label_count - 1) * line_spacing;

    int start_y = (screen_h - total_h) / 2;

    int title_x = (screen_w - title_w) / 2;
    int title_y = start_y;

    Position button_pos[] = {
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing
        },
        {
            (screen_w - max_button_size.w) / 2,
            title_y + title.font_size + line_spacing + max_button_size.h + line_spacing
        }
    };

    Size title_size = {
        (int)MeasureTextStyled(title).x,
        (int)MeasureTextStyled(title).y
    };

    int frame_pad_x = 30;
    int frame_pad_y = 25;

    int content_w = (title_size.w > max_button_size.w) ? title_size.w : max_button_size.w;
    int content_h = title_size.h + line_spacing + label_count * max_button_size.h + (label_count - 1) * line_spacing;

    Rectangle frame = {
        (float)((screen_w - (content_w + 2 * frame_pad_x)) / 2),
        (float)((screen_h - (content_h + 2 * frame_pad_y)) / 2),
        (float)(content_w + 2 * frame_pad_x),
        (float)(content_h + 2 * frame_pad_y)
    };
    float roundness = 0.20f, thick = 2.0f;
    int segments = 32;
    DrawRectangleRounded(frame, roundness, segments, COLOR_MENU);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, thick, darken(COLOR_MENU, 0.20f));

    DrawTextStyled(title, (Position){title_x, title_y});
    
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    button_menu(labels[0], button_pos[0], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_main_menu, g});
    button_menu(labels[1], button_pos[1], max_button_size, COLOR_MENU_BUTTON, (ButtonAction){on_exit, &g->status});
}
