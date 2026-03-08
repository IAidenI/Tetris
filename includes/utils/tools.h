#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncursesw/curses.h>

typedef struct {
    int x;
    int y;
} Position;

void shuffle_array(void *arr, int n, size_t size);
double get_time();
void handle_sigint(int sig);
void help(const char* buffer);

#endif // TOOLS_H
