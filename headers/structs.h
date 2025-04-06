#ifndef STRUCTS_H
#define STRUCTS_H

#include "game_settings.h"

#define BLOCK_COUNT 8

// Pour la position du block
typedef struct {
    int x;
    int y;
} Position;

// Pour toutes les informations liés au scoring
typedef struct {
    int score;
    int level;
    int nb_lines;
    float speed;
} GameState;

// Pour les infos sur la game en cours
typedef struct {
    Position pos;
    
    int **block;
    int id_block;
    int direction;

    int **next_block;
    int id_next_block;

    int grid[GAME_API_HEIGHT][GAME_API_WIDTH];
    
    GameState state;

    int seven_bag[BLOCK_COUNT - 1];

    int flag; // Un flag global pour vérifier des conditions 
              // (principalement pour savoir si c'est le début de la game)
} APIGame;

// Pour le flag
#define FLAG_NEUTRAL 0
#define FLAG_START   1
#define FLAG_DEBUG   2

#endif