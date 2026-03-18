#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "core/game.h"
#include "ui/ncurses/display.h"

#define BUFFER_SNAPSHOT 32

void snapshot_init(const char *path);
int  snapshot_read(Game *g);
void snapshot_create(Game *g);

#endif // SNAPSHOT_H
