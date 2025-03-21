#ifndef GAME_H
#define GAME_H

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "colors.h"
#include "print.h"
#include "function_ret.h"
#include "block.h"

// Pour la position du block
typedef struct {
    int x;
    int y;
} Position;

// Pour les infos sur la game en cours
typedef struct {
    Position pos;
    
    int **block;
    int id_block;
    int direction;

    int **next_block;
    int id_next_block;

    int **grid;
} APIGame;
#define APIGAME_WALL 9

#define IS_BLOCK     1
#define IS_NEXT_BLOCK 2

// Dimensions du jeu
// Pour les dimensions on prend en compte les bordures
#define GAME_WEIGHT_MUL 2
#define GAME_HEIGHT (20 + 2) // Les 2 bordures
#define GAME_WEIGHT (GAME_WEIGHT_MUL * 10 + 2) // Car une case c'est [] donc 2 caractères donc 2 * et les 2

#define GAME_API_WEIGHT ((GAME_WEIGHT - 2) / GAME_WEIGHT_MUL + 2)
#define GAME_API_HEIGHT GAME_HEIGHT

#define NEXT_BLOCK_HEIGHT 6
#define NEXT_BLOCK_WEIGHT 14
#define BLOCK_WAIT 800 // 800 pour le classique
#define NEXT_BLOCK_IHM_LEN 12

// Pour savoir la direction où on veux se déplacer
#define GO_LEFT  15
#define GO_RIGHT 16
#define GO_DOWN  17
#define GO_UP    18

// Gestion recuperation pos bloc
#define Get_Start_Of_Block(game) Get_X_Of_Block(game, 1)
#define Get_End_Of_Block(game) Get_X_Of_Block(game, 0)

void Cancel_Rotate(APIGame *game);
void Rotate_Block(APIGame *game);
int Get_X_Of_Block(APIGame *game, const int first);
int Get_Block_Width(APIGame *game);

int Set_Block(APIGame *game, int block, const int old_size);

void Refresh_Grid(APIGame *game);

int Is_Colision(APIGame *game);
int Block_Physics(APIGame *game);

int Spawn(APIGame *game);
int Start_Game(APIGame *game);
void Stop_Game(APIGame *game);


void Display_Block(const char *text, APIGame *game);
void Display_Next_Block(const char *text, APIGame *game);
void Display_Grid(const char *text, APIGame *game);

#endif
