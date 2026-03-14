#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "core/game.h"
#include "ui/ncurses/display.h"

#define BUFFER_SNAPSHOT 32

void snapshot_init(const char *path);
int  snapshot_search_key_word(FILE *fp, const char *key_word);
int  snapshot_extract_section_int(FILE *fp, char *buffer, const char *section, const char *field, int *out);
int  snapshot_extract_array(FILE *fp, char *buffer, const char *keyword, int *out, int width, int height);
void snapshot_extract();
int  snapshot_read(Game *g);
void snapshot_create(Game *g);

#endif // SNAPSHOT_H
