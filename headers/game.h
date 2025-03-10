#ifndef GAME_H
#define GAME_H

#define _XOPEN_SOURCE_EXTENDED 1 // Pour afficher l'UTF-8
#include <locale.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "colors.h"
#include "print.h"
#include "function_ret.h"

// Structure pour associer un bloc à une couleur
typedef struct {
    const wchar_t *shape;
    int color;
} Block;

// Pour la position du block
typedef struct {
    int x;
    int y;
} Position;

// Pour les infos sur la game en cours
typedef struct {
    Position pos;
    Block block;
    int id_block;
    Block next_block;
    int id_next_block;
    wchar_t **lines;
} APIGame;

// Caractères UTF-8 pour le carde
#define MIDLESCORE       L"\u2500"
#define CORNER_TOP_LEFT  L"\u256D"
#define CORNER_TOP_RIGHT L"\u256e"
#define CORNER_BOT_LEFT  L"\u2570"
#define CORNER_BOT_RIGHT L"\u256f"
#define WALL             L"\u2502"
#define CROSS            L"\u253C"
#define CROSS_TOP        L"\u252C"
#define CROSS_BOT        L"\u2534"
#define CROSS_LEFT       L"\u251C"
#define CROSS_RIGHT      L"\u2524"

// Blocs pour la game
#define BLOCK_I L"        \n[][][][]\n        \n        "
#define BLOCK_J L"[]    \n[][][]\n      "
#define BLOCK_L L"    []\n[][][]\n      "
#define BLOCK_O L"  [][]\n  [][]\n      "
#define BLOCK_S L"  [][]\n[][]  \n      "
#define BLOCK_T L"  []  \n[][][]\n      "
#define BLOCK_Z L"[][]  \n  [][]\n      "
#define MAX_COPY 40
#define BLOCK_CHAR L"[]"

// Cadre de la game
#define Top_Border() Borders(CORNER_TOP_LEFT, CORNER_TOP_RIGHT)
#define Bottom_Border() Borders(CORNER_BOT_LEFT, CORNER_BOT_RIGHT)

void IsLoose(APIGame *game);
int Get_True_Pos(APIGame *game);
size_t Block_Size(const wchar_t *shape);
size_t Block_Max_Lenth(const int i);
void Put_Next_Block(APIGame *game);
void Put_Block(APIGame *game);
void Del_Block(APIGame *game);
int Place_Block(APIGame *game);
void Borders(const wchar_t *c1, const wchar_t *c2);
void Create_Frame();

// Dimensions du jeu
// Pour les dimensions on ne prends pas en compte les bordures
#define GAME_HEIGHT 20
#define GAME_WEIGHT 2 * 10 // Car une case c'est [] donc 2 caractères
#define NEXT_BLOCK_HEIGHT 4
#define NEXT_BLOCK_WEIGHT 12
#define BLOCK_WAIT 100
#define NEXT_BLOCK_IHM_LEN 12

void Spawn(APIGame *game, const int id_block);
int Game(const char *block_choose);

#endif
