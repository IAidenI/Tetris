#ifndef GAME_H
#define GAME_H

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "print.h"
#include "return_code.h"
#include "block.h"
#include "structs.h"
#include "game_settings.h"

// Permet à certaines fonctions de savoir si game->block est le bloc actuelle ou le suivant
#define IS_BLOCK     1
#define IS_NEXT_BLOCK 2

// Pour récuperer le début ou la fin d'un bloc
#define Get_Start_Of_Block(game) Get_X_Of_Block(game, 1)
#define Get_End_Of_Block(game) Get_X_Of_Block(game, 0)

// Game
int Start_Game(APIGame *game);
void Stop_Game(APIGame *game);

// Relatif à la création d'un bloc
int Spawn(APIGame *game);
int Random_Block(APIGame *game);
int Set_Block(APIGame *game, int block, const int old_size);


// Relatif au colisions
int Block_Physics(APIGame *game);
int Is_Colision(APIGame *game);

// Relatif aux rotations de blocs
void Rotate_Block(APIGame *game);
void Cancel_Rotate(APIGame *game);


// Récuperation d'information sur un bloc
int Get_X_Of_Block(APIGame *game, const int first);
int Get_Block_Width(APIGame *game);


// Relatif au scoring
const char *Get_Difficulty(int level);
void Compute_Score(GameState *state, int lines);
int Compute_Gravity(GameState *state);


// Relatif à au 7-bag
void Fill_Bag(APIGame *game);
int Is_Bag_Empty(APIGame *game);


// Relatif au snapshot
int Search_Key_Word(FILE *fp, const char *key_word);
int Set_Game(APIGame *game, const char *path_name);


// Relatif à la linge full
int Clear_Full_Lines(APIGame *game);


// Relatif à l'ajout d'un bloc
void Update_Block_API(APIGame *game, const int x, const int y, const int value, const wchar_t *state);

#endif
