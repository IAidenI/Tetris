#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "core/game.h"
#include "ui/ncurses/display.h"

#define BUFFER_SNAPSHOT 32
#define ERROR  1
#define SUCCESS 0

void snapshot_init(const char *path);
int snapshot_extract_int(FILE *fp, char *buffer, const char *keyword, int *out);
int snapshot_extract_array(FILE *fp, char *buffer, const char *keyword, int *out, int width, int height);
void snapshot_extract();
int snapshot_read(Game *g);
void snapshot_create(Game *g);

#endif // SNAPSHOT_H
