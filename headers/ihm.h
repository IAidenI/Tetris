#ifndef IHM_H
#define IHM_H

#define _XOPEN_SOURCE_EXTENDED 1 // Pour afficher l'UTF-8
#include <ncurses.h>
#include <locale.h>

#include "game.h"
#include "block.h"

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

// Gestion affichage d'un bloc
#define Put_Block(game, x, y) Update_Block(game, x, y, BLOCK)
#define Del_Block(game, x, y) Update_Block(game, x, y, NO_BLOCK)

// Cadre de la game
#define Top_Border() Borders(CORNER_TOP_LEFT, CORNER_TOP_RIGHT)
#define Bottom_Border() Borders(CORNER_BOT_LEFT, CORNER_BOT_RIGHT)

void Print_Shape(int shape[BLOCK_SIZE][BLOCK_SIZE]);
void Del_Next_Block(APIGame *game, const int posX, const int posY);
void Put_Next_Block(APIGame *game);
void Update_Block(APIGame *game, int posX, int posY, const wchar_t *state);
int Place_Block(APIGame *game, const int direction);

void Borders(const wchar_t *c1, const wchar_t *c2);
void Create_Frame();

int Spawn(APIGame *game);
int Game();

#endif
