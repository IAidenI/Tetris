#include "utils/snapshot.h"

static const char *snapshot_path = NULL;

// Key words
static const char *key_current_position = "[position]";
static const char *key_current_type = "[current_type]";
static const char *key_current_shape = "[current_shape]";
static const char *key_next_type = "[next_type]";
static const char *key_grid = "[grid]";

void snapshot_init(const char *path) {
    snapshot_path = path;
}

int snapshot_search_key_word(FILE *fp, const char *key_word) {
    char buffer[BUFFER_SNAPSHOT];
    rewind(fp);

    while (fgets(buffer, BUFFER_SNAPSHOT, fp)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strcmp(buffer, key_word) == 0) return SUCCESS;
    }
    return ERROR;
}

int snapshot_extract_int(FILE *fp, char *buffer, const char *keyword, int *out) {
    if (snapshot_search_key_word(fp, keyword)) {
        print_error("Format du fichier invalide, %s introuvable.\n", keyword);
        return ERROR;
    }

    *out = -1;
    while (*out == -1) {
        if (!fgets(buffer, BUFFER_SNAPSHOT, fp)) {
            print_error("Format du fichier invalide, %s trouvé, mais aucune donnée.\n", keyword);
            return ERROR;
        }

        char *line = buffer;
        while (*line == ' ' || *line == '\t')
            line++;

        sscanf(line, "%*s %*s %d", out);
    }
    return SUCCESS;
}

int snapshot_extract_array(FILE *fp, char *buffer, const char *keyword, int *out, int width, int height) {
    if (snapshot_search_key_word(fp, keyword)) {
        print_error("Format du fichier invalide, %s introuvable.\n", keyword);
        return 1;
    }

    for (int y = 0; y < height; y++) {
        if (!fgets(buffer, BUFFER_SNAPSHOT, fp)) {
            print_error("Format du fichier invalide, %s trouvé, mais aucune donnée ligne %d.\n", keyword, y);
            return 1;
        }

        char *ptr = buffer, *end;
        for (int x = 0; x < width; x++) {
            int value = (int)strtol(ptr, &end, 10);
            if (ptr == end) {
                print_error("Valeur invalide à la ligne %d, colonne %d.\n", y + 1, x + 1);
                return 1;
            }
            out[x * height + y] = value;
            ptr = end;
        }
    }
    return 0;
}

int snapshot_read(Game *g) {
    if (snapshot_path == NULL) {
        log_write("[!] snapshot_init not called.\n");
        return 1;
    }

    char buffer[BUFFER_SNAPSHOT];
    int x = -1, y = -1, current_type = -1, next_type = -1;
    int shape[TETROMINO_SIZE][TETROMINO_SIZE];
    int grid[GRID_WIDTH][GRID_HEIGHT];

    FILE *fp = fopen(snapshot_path, "r");

    if (snapshot_extract_int(fp, buffer, key_current_type, &current_type) == ERROR) return ERROR;
    if (snapshot_extract_int(fp, buffer, key_current_position, &x) == ERROR) return ERROR;
    if (snapshot_extract_int(fp, buffer, key_current_position, &y) == ERROR) return ERROR;
    if (snapshot_extract_array(fp, buffer, key_current_shape, (int *)shape, TETROMINO_SIZE, TETROMINO_SIZE) == ERROR) return ERROR;
    if (snapshot_extract_int(fp, buffer, key_next_type, &next_type) == ERROR) return ERROR;
    if (snapshot_extract_array(fp, buffer, key_grid, (int *)grid, GRID_WIDTH, GRID_HEIGHT) == ERROR) return ERROR;

    // Update the game
    g->current = tetromino_get(current_type);                   // Current type
    g->current.pos.x = x; g->current.pos.y = y;                 // Current position
    memcpy(&g->current.shape, shape, sizeof(g->current.shape)); // Current shape
    g->next = tetromino_get(next_type);                         // Next type
    memcpy(&g->grid, grid, sizeof(g->grid.cell));               // Grid
    g->status = SNAPSHOT;

    return SUCCESS;
}

void snapshot_create(Game *g) {
    if (snapshot_path == NULL) {
        log_write("[!] snapshot_init not called.\n");
        return;
    }

    FILE *fp = fopen(snapshot_path, "w");

    // Current tetromino type
    fprintf(fp, "%s\n", key_current_type);
    fprintf(fp, "    type : %d\n\n", g->current.type);

    // Current tetromino position
    fprintf(fp, "%s\n", key_current_position);
    fprintf(fp, "    x : %d\n", g->current.pos.x);
    fprintf(fp, "    y : %d\n\n", g->current.pos.y);

    // Current tetromino shape
    fprintf(fp, "%s\n", key_current_shape);
    for (int h = 0; h < TETROMINO_SIZE; h++) {
        fprintf(fp, "    ");
        for (int w = 0; w < TETROMINO_SIZE; w++) {
            fprintf(fp, "%d ", g->current.shape[w][h]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    // Next tetromino
    fprintf(fp, "%s\n", key_next_type);
    fprintf(fp, "    type : %d\n\n", g->next.type);

    // Grid
    fprintf(fp, "%s\n", key_grid);
    for (int h = 0; h < GRID_HEIGHT; h++) {
        fprintf(fp, "    ");
        for (int w = 0; w < GRID_WIDTH; w++) {
            fprintf(fp, "%d ", g->grid.cell[w][h]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    fclose(fp);
}
