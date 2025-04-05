#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

// Dimensions du jeu (on prend en compte les bordures)
/*
    Il faut différencier 2 choses :
        - L'API qui à les dimensions de base d'un Tetris (donc 10x20)
        - L'affichage qui peux avoir une dimension différente (ici un bloc corresond à [], donc c'est 2 fois plus large que pour l'API)
*/
#define GAME_API_WIDTH  (10 + 2) // + 2 à cause des bordures
#define GAME_API_HEIGHT (20 + 2)

#define GAME_WIDTH_MUL 2
#define GAME_HEIGHT GAME_API_HEIGHT
#define GAME_WIDTH (GAME_WIDTH_MUL * (GAME_API_WIDTH - 2) + 2)

// Les dimensions de l'encadré du bloc suivant
#define NEXT_BLOCK_HEIGHT 6
#define NEXT_BLOCK_WIDTH 14
#define NEXT_BLOCK_IHM_LEN 12

// Définit le caractère utiliser dans l'API pour représenter le bord de la grille
#define APIGAME_WALL 9

// Pour savoir la direction de l'utilisateur
#define GO_LEFT  15
#define GO_RIGHT 16
#define GO_DOWN  17
#define GO_UP    18

#endif