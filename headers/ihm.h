#ifndef IHM_H
#define IHM_H

#define _XOPEN_SOURCE_EXTENDED 1 // Pour afficher l'UTF-8
#include <ncurses.h>
#include <time.h>
#include <locale.h>

#include "menu.h"
#include "block.h"
#include "game.h"

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

// Game
int Game(APIGame *game);
int Get_New_Block(APIGame *game);


// Relatif à la création du visuel
void Create_Frame();
void Create_Frame_Next_Block();
void Create_Info();
void Borders(const wchar_t *c1, const wchar_t *c2);


// Relatif à l'affichage d'un bloc
int Place_Block(APIGame *game);
void Update_Block(APIGame *game, int posX, int posY, const wchar_t *state);

int Put_Next_Block(APIGame *game);
void Del_Next_Block(APIGame *game, const int posX, const int posY);


// Opérations sur des blocs
int **Clone_Block(int **src, int size);
void Free_Block(int **block, int size);


// Relatif à la grille
void Refresh_Game(APIGame *game);
void Refresh_Grid(APIGame *game);


// Relatif au scoring
void Put_Score(int score);
void Put_Level(int level);


// Relatif à la linge full
int Clear_Full_Lines(APIGame *game);

#endif
