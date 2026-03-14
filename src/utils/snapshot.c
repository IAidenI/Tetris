#include "utils/snapshot.h"

static const char *snapshot_path = NULL;

// Key words
static const char *key_current_position = "[current_position]";
static const char *key_current_type     = "[current_type]";
static const char *key_current_shape    = "[current_shape]";
static const char *key_next_type        = "[next_type]";
static const char *key_grid             = "[grid]";
static const char *key_game_status      = "[game_status]";

// Labels
static const char *label_position_x   = "x";
static const char *label_position_y   = "y";
static const char *label_current_type = "type";
static const char *label_next_type    = "type";
static const char *label_game_score   = "score";
static const char *label_game_level   = "level";

void snapshot_init(const char *path) {
    snapshot_path = path;
}

int snapshot_search_key_word(FILE *fp, const char *key_word) {
    char buffer[BUFFER_SNAPSHOT];
    rewind(fp);

    while (fgets(buffer, BUFFER_SNAPSHOT, fp)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strcmp(buffer, key_word) == 0) return 0;
    }
    return 1;
}

int snapshot_extract_section_int(FILE *fp, char *buffer, const char *section, const char *field, int *out) {
    if (snapshot_search_key_word(fp, section)) {
        print_error("Format du fichier invalide, %s introuvable.\n", section);
        return 1;
    }

    while (fgets(buffer, BUFFER_SNAPSHOT, fp)) {
        char *line = buffer;

        while (*line == ' ' || *line == '\t')
            line++;

        if (*line == '[')
            break;

        char key[BUFFER_SNAPSHOT];
        int value;

        if (sscanf(line, "%[^:]: %d", key, &value) == 2) {
            char *end = key + strlen(key) - 1;
            while (end >= key && (*end == ' ' || *end == '\t')) {
                *end = '\0';
                end--;
            }

            if (strcmp(key, field) == 0) {
                *out = value;
                return 0;
            }
        }
    }

    print_error("Champ %s introuvable dans %s.\n", field, section);
    return 1;
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
            out[y * width + x] = value;
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
    int x = -1, y = -1, current_type = -1, next_type = -1, score = -1, level = -1;
    int shape[TETROMINO_SIZE][TETROMINO_SIZE];
    int grid[GRID_HEIGHT][GRID_WIDTH];

    FILE *fp = fopen(snapshot_path, "r");
	if (fp == NULL) {
		log_write("[!] can't read %s.\n", snapshot_path);
		return 1;
	}

    if (snapshot_extract_section_int(fp, buffer, key_current_type, label_current_type, &current_type) == 1) return 1;
	
    if (snapshot_extract_section_int(fp, buffer, key_current_position, label_position_x, &x)) return 1;
    if (snapshot_extract_section_int(fp, buffer, key_current_position, label_position_y, &y)) return 1;
    
    if (snapshot_extract_array(fp, buffer, key_current_shape, (int *)shape, TETROMINO_SIZE, TETROMINO_SIZE) == 1) return 1;
    
    if (snapshot_extract_section_int(fp, buffer, key_next_type, label_next_type, &next_type) == 1) return 1;
    
    if (snapshot_extract_array(fp, buffer, key_grid, (int *)grid, GRID_WIDTH, GRID_HEIGHT) == 1) return 1;
    
    if (snapshot_extract_section_int(fp, buffer, key_game_status, label_game_score, &score)) return 1;
    if (snapshot_extract_section_int(fp, buffer, key_game_status, label_game_level, &level)) return 1;

    // Update the game
    g->current = tetromino_get(current_type);                   // Current type
    g->current.pos.x = x; g->current.pos.y = y;                 // Current position
    memcpy(&g->current.shape, shape, sizeof(g->current.shape)); // Current shape
    g->next = tetromino_get(next_type);                         // Next type
    memcpy(&g->grid, grid, sizeof(g->grid.cell));               // Grid
    g->grid.total_lines_cleared = level * 10;                   // Grid total
    g->score = score;                                           // Score
    g->level = level;                                           // Level
    g->status = SNAPSHOT;

    return 0;
}

void snapshot_create(Game *g) {
    if (snapshot_path == NULL) {
        log_write("[!] snapshot_init not called.\n");
        return;
    }

    FILE *fp = fopen(snapshot_path, "w");

    // Current tetromino type
    fprintf(fp, "%s\n", key_current_type);
    fprintf(fp, "    %s : %d\n\n", label_current_type, g->current.type);

    // Current tetromino position
    fprintf(fp, "%s\n", key_current_position);
    fprintf(fp, "    %s : %d\n", label_position_x, g->current.pos.x);
    fprintf(fp, "    %s : %d\n\n", label_position_y, g->current.pos.y);

    // Current tetromino shape
    fprintf(fp, "%s\n", key_current_shape);
    for (int h = 0; h < TETROMINO_SIZE; h++) {
        fprintf(fp, "    ");
        for (int w = 0; w < TETROMINO_SIZE; w++) {
            fprintf(fp, "%d ", g->current.shape[h][w]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    // Next tetromino
    fprintf(fp, "%s\n", key_next_type);
    fprintf(fp, "    %s : %d\n\n", label_next_type, g->next.type);

    // Grid
    fprintf(fp, "%s\n", key_grid);
    for (int h = 0; h < GRID_HEIGHT; h++) {
        fprintf(fp, "    ");
        for (int w = 0; w < GRID_WIDTH; w++) {
            fprintf(fp, "%d ", g->grid.cell[h][w]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    // Game status
    fprintf(fp, "%s\n", key_game_status);
    fprintf(fp, "    %s : %d\n", label_game_score, g->score);
    fprintf(fp, "    %s : %d\n\n", label_game_level, g->level);

    fclose(fp);
}
