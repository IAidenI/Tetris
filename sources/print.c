#include "../headers/print.h"

void Display_Block(const char *text, APIGame *game) {
    Debug("%s\n", text);
    Debug("id du bloc : %d\n", game->id_block);
    Debug("position : x : %d - y : %d\n", game->pos.x, game->pos.y);
    int size = Get_Block_Size(game->id_block);
    for (int i = 0; i < size; i++) {
        Debug();
        for (int j = 0; j < size; j++) {
            DebugSimple("%d ", game->block[i][j]);
        }
        DebugSimple("\n");
    }
    DebugSimple("\n");
}

void Display_Next_Block(const char *text, APIGame *game) {
    Debug("%s\n", text);
    Debug("id du bloc : %d\n", game->id_next_block);
    int size = Get_Block_Size(game->id_next_block);
    for (int i = 0; i < size; i++) {
        Debug();
        for (int j = 0; j < size; j++) {
            DebugSimple("%d ", game->next_block[i][j]);
        }
        DebugSimple("\n");
    }
    DebugSimple("\n");
}

void Display_Grid(const char *text, APIGame *game) {
    Debug("%s\n", text);
    for (int i = 0; i < GAME_API_HEIGHT; i++) {
        Debug();
        for (int j = 0; j < GAME_API_WIDTH; j++) {
            DebugSimple("%d ", game->grid[i][j]);
        }
        DebugSimple("\n");
    }
    DebugSimple("\n");
}

void Display_Bag(const char *text, APIGame *game) {
    Debug("%s\n", text);
    for (int i = 0; i < BLOCK_COUNT - 1; i++) {
        DebugSimple("%d ", game->seven_bag[i]);
    }
    DebugSimple("\n");
}