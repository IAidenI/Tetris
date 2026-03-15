#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ncursesw/curses.h>

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int w;
    int h;
} Size;

#define print_error(text, ...) fprintf(stderr, "Erreur: " text, ##__VA_ARGS__); fflush(stderr);

double get_time();
void shuffle_array(void *arr, int n, size_t size);
void handle_sigint(int sig);
void help(const char* buffer);
int  file_exists(const char *filename);

#endif // TOOLS_H
