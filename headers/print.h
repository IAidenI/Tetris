#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>

#include "structs.h"
#include "block.h"

#define Error(text, ...) fprintf(stderr, "Erreur: " text, ##__VA_ARGS__); fflush(stderr);

// On redirige les info de debug vers un fichier
#define InitDebug() \
    do { \
        FILE *__fp = fopen("/tmp/debug_tetris.log", "w"); \
        if (__fp) fclose(__fp); \
    } while (0)

// Avec [nom_function] au début
#define Debug(text, ...) \
    do { \
        FILE *__fp = fopen("/tmp/debug_tetris.log", "a"); \
        if (__fp) { \
            fprintf(__fp, "[%s] : " text, __func__ , ##__VA_ARGS__); \
            fclose(__fp); \
        } \
    } while (0)

// Sans [nom_function] au début
#define DebugSimple(text, ...) \
    do { \
        FILE *__fp = fopen("/tmp/debug_tetris.log", "a"); \
        if (__fp) { \
            fprintf(__fp, text, ##__VA_ARGS__); \
            fclose(__fp); \
        } \
    } while (0)


#define BUFFER_DEBUG 32
// Fonctions pour afficher afficher des informations de debug
void Display_Block(const char *text, APIGame *game);
void Display_Next_Block(const char *text, APIGame *game);
void Display_Grid(const char *text, APIGame *game);
void Display_Bag(const char *text, APIGame *game);

// Informations supplémentaire pour le debug
#define SNAPSHOT_FILE "./snapshot"

#endif
