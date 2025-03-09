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
    Block next_block;
    wchar_t **lines;
} APIGame;

// Caractères UTF-8 pour le carde
#define MIDLESCORE       L"\u2500"
#define CORNER_TOP_LEFT  L"\u256D"
#define CORNER_TOP_RIGHT L"\u256e"
#define CORNER_BOT_LEFT  L"\u2570"
#define CORNER_BOT_RIGHT L"\u256f"
#define WALL             L"\u2502"

// Blocs pour la game
#define BLOCK_I L"[][][][]"
#define BLOCK_J L"[]\n[][][]"
#define BLOCK_L L"    []\n[][][]"
#define BLOCK_O L"[][]\n[][]"
#define BLOCK_S L"  [][]\n[][]"
#define BLOCK_T L"  []\n[][][]"
#define BLOCK_Z L"[][]\n  [][]"
#define MAX_COPY 32

// Cadre de la game
#define Top_Border() Borders(CORNER_TOP_LEFT, CORNER_TOP_RIGHT)
#define Bottom_Border() Borders(CORNER_BOT_LEFT, CORNER_BOT_RIGHT)

size_t Block_Size(const wchar_t *shape);
size_t Block_Max_Lenth(const int i);
int Put_Block(APIGame *game);
void Borders(const wchar_t *c1, const wchar_t *c2);
void Create_Frame();

// Dimensions du jeu
#define GAME_HEIGHT 20
#define GAME_WEIGHT 2 * 10 // Car une case c'est [] donc 2 caractères
#define BLOCK_WAIT 100

void Spawn(APIGame *game);
int Game();

#endif
