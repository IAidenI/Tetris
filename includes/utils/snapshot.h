#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "core/game.h"

#define BUFFER_SNAPSHOT 32

void snapshot_init(const char *path);
const char *snapshot_get_message();
MessageLevel snapshot_get_message_level();
int  snapshot_read(Game *g);
void snapshot_create(Game *g);

#endif // SNAPSHOT_H
