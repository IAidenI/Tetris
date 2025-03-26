#ifndef PRINT_H
#define PRINT_H

#define Error(text, ...) fprintf(stderr, "Erreur: " text, ##__VA_ARGS__); fflush(stderr);

// On redirige les info de débug vers un fichier
#define InitDebug() \
    do { \
        FILE *__fp = fopen("/tmp/debug_tetris.log", "w"); \
        if (__fp) fclose(__fp); \
    } while (0)

#define Debug(text, ...) \
    do { \
        FILE *__fp = fopen("/tmp/debug_tetris.log", "a"); \
        if (__fp) { \
            fprintf(__fp, "[%s] : " text, __func__ , ##__VA_ARGS__); \
            fclose(__fp); \
        } \
    } while (0)

#define DebugSimple(text, ...) \
    do { \
        FILE *__fp = fopen("/tmp/debug_tetris.log", "a"); \
        if (__fp) { \
            fprintf(__fp, text, ##__VA_ARGS__); \
            fclose(__fp); \
        } \
    } while (0)

#endif
