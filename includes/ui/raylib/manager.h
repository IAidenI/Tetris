#ifndef MANAGER_H
#define MANAGER_H

#include <raylib.h>
#include <uchar.h>
#include "utils/tools.h"

#define ENTITY_FONT     "assets/fonts/ByteBounce.ttf"
#define DIALOGUE_FONT   "assets/fonts/Jersey10-Regular.ttf"
#define INFO_FONT       "assets/fonts/Inconsolata-Regular.ttf"
#define SMALL_SIZE      20
#define BIG_SIZE        50
#define FONT_SIZE_COUNT 2
// ASCII + accents FR
static const char32_t FR_CHARS[] = U" !\"#$%&'()*+,-./0123456789:;<=>?@"
                                   U"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                                   U"abcdefghijklmnopqrstuvwxyz{|}~"
                                   U"àâäçéèêëîïôöùûüÿÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸ’«»œŒ";
#define FR_CHARS_COUNT ((int)(sizeof(FR_CHARS) / sizeof(FR_CHARS[0]) - 1))

typedef enum {
    TEX_MAX // Sentinel to check the size
} TextureID;

typedef enum {
    ENTITY_LABEL,
    DIALOGUE_LABEL,
    INFO_LABEL,
    // Sentinel to check the size
    FONT_MAX
} FontID;

typedef struct {
    Texture2D tmgr[TEX_MAX];
    Font fmgr[FONT_MAX][FONT_SIZE_COUNT];
} Manager;

int manager_init(Manager *m);
Texture2D manager_get_texture(const Manager *m, TextureID id);
const Font *manager_get_font(const Manager *m, FontID id, int size);
void manager_cleanup(Manager *m);

#endif // MANAGER_H
