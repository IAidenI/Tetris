#include "ui/raylib/manager.h"

int manager_init(Manager *m) {
    const char *textures[TEX_MAX];
    const char *fonts[FONT_MAX] = {
        ENTITY_FONT,
        DIALOGUE_FONT,
        INFO_FONT
    };

    // Check if files exists
    for (int i = 0; i < TEX_MAX; i++) {
        if (!file_exists(textures[i])) {
            print_error("Le fichier %s est introuvable.\n", textures[i]);
            return 1;
        }
    }

    for (int i = 0; i < FONT_MAX; i++) {
        if (!file_exists(fonts[i])) {
            print_error("Le fichier %s est introuvable.\n", fonts[i]);
            return 1;
        }
    }

    // Initisalize textures
    for (int i = 0; i < TEX_MAX; i++) m->tmgr[i] = LoadTexture(textures[i]);

    // Initisalize fonts
    int cps[FR_CHARS_COUNT];
    for (int i = 0; i < FR_CHARS_COUNT; i++) cps[i] = (int)FR_CHARS[i];
    
    for (int i = 0; i < FONT_MAX; i++) {
        m->fmgr[i][0] = LoadFontEx(fonts[i], SMALL_SIZE, cps, FR_CHARS_COUNT);
        SetTextureFilter(m->fmgr[i][0].texture, TEXTURE_FILTER_BILINEAR);

        m->fmgr[i][1] = LoadFontEx(fonts[i], BIG_SIZE, cps, FR_CHARS_COUNT);
        SetTextureFilter(m->fmgr[i][1].texture, TEXTURE_FILTER_BILINEAR);
    }
    return 0;
}

Texture2D manager_get_texture(const Manager *m, TextureID id) {
    return m->tmgr[id];
}

const Font *manager_get_font(const Manager *m, FontID id, int size) {
    int index = (size <= SMALL_SIZE) ? 0 : 1;
    return &m->fmgr[id][index];
}

void manager_cleanup(Manager *m) {
    // Unload textures
    for (int i = 0; i < TEX_MAX; i++) {
        UnloadTexture(m->tmgr[i]);
    }

    // Unload fonts
    for (int i = 0; i < FONT_MAX; i++) {
        for (int j = 0; j < FONT_SIZE_COUNT; j++) {
            UnloadFont(m->fmgr[i][j]);
        }
    }
}
