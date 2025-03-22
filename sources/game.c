#include "../headers/game.h"

void Display_Block(const char *text, APIGame *game) {
    Debug("%s\n", text);
    Debug("id du bloc : %d\n", game->id_block);
    Debug("position : x : %d - y : %d\n", game->pos.x, game->pos.y);
    int size = Get_Block_Size(game->id_block);
    for (int i = 0; i < size; i++) {
        Debug();
        for (int j = 0; j < size; j++) {
            DebugSimple("%d ", game->block[i][j]);
        }
        DebugSimple("\n");
    }
    DebugSimple("\n");
}

void Display_Next_Block(const char *text, APIGame *game) {
    Debug("%s\n", text);
    Debug("id du bloc : %d\n", game->id_next_block);
    int size = Get_Block_Size(game->id_next_block);
    for (int i = 0; i < size; i++) {
        Debug();
        for (int j = 0; j < size; j++) {
            DebugSimple("%d ", game->next_block[i][j]);
        }
        DebugSimple("\n");
    }
    DebugSimple("\n");
}

void Display_Grid(const char *text, APIGame *game) {
    Debug("%s\n", text);
    for (int i = 0; i < GAME_API_HEIGHT; i++) {
        Debug();
        for (int j = 0; j < GAME_API_WEIGHT; j++) {
            DebugSimple("%d ", game->grid[i][j]);
        }
        DebugSimple("\n");
    }
    DebugSimple("\n");
}

int Search_Key_Word(FILE *fp, const char *key_word) {
    char buffer[BUFFER_DEBUG];
    rewind(fp); // On remet la tête de lecture au début du fichier

    while (fgets(buffer, BUFFER_DEBUG, fp)) {
        buffer[strcspn(buffer, "\r\n")] = 0; // On enlève le \r et/ou \n

        // On vérifie si il correspond au key word
        if (strcmp(buffer, key_word) == 0) {
            return SUCCESS;
        }
    }
    return ERROR;
}

int Set_Game(APIGame *game, const char *path_name) {
    // Liste des key words
    const char *position = "[position]";
    const char *current_block = "[current block]";
    const char *next_block = "[next block]";
    const char *grid_key = "[grid]";

    char buffer[BUFFER_DEBUG];
    int x = -1, y = -1, current_id = -1, next_id = -1;
    int grid[GAME_API_HEIGHT][GAME_API_WEIGHT];

    FILE *fp = fopen(path_name, "r");
    if (!fp) {
        Error("N'as pas réussit à lire le fichier.\n");
        return ERROR;
    }

    // On commencer par chercher la position du bloc actuelle dans le fichier
    if (Search_Key_Word(fp, position)) {
        Error("Format du fichier invalide, %s introuvable.\n", position);
        return ERROR;
    }

    // On extrait les positions
    while (x == -1 || y == -1) {
        if (!fgets(buffer, BUFFER_DEBUG, fp)) {
            Error("Format du fichier invalide, %s trouvé, mais aucune données à chargé trouvé (%s).\n", position, buffer);
            return ERROR;
        }

        // On enlève les espaces au début
        char *line = buffer;
        while (*line == ' ' || *line == '\t') line++;

        if (sscanf(line, "x : %d", &x) == 1) {
            continue;
        }

        if (sscanf(line, "y : %d", &y) == 1) {
            continue;
        }
    }

    // On cherche ensuite l'id du bloc actuelle
    if (Search_Key_Word(fp, current_block)) {
        Error("Format du fichier invalide, %s introuvable.\n", current_block);
        return ERROR;
    }

    // On extrait l'id
    while (current_id == -1) {
        if (!fgets(buffer, BUFFER_DEBUG, fp)) {
            Error("Format du fichier invalide, %s trouvé, mais aucune données à chargé trouvé.\n", current_block);
            return ERROR;
        }

        // On enlève les espaces au début
        char *line = buffer;
        while (*line == ' ' || *line == '\t') line++;

        if (sscanf(line, "id : %d", &current_id) == 1) {
            continue;
        }
    }

    // On cherche l'id du bloc suivant
    if (Search_Key_Word(fp, next_block)) {
        Error("Format du fichier invalide, %s introuvable.\n", next_block);
        return ERROR;
    }

    // On extrait l'id
    while (next_id == -1) {
        if (!fgets(buffer, BUFFER_DEBUG, fp)) {
            Error("Format du fichier invalide, %s trouvé, mais aucune données à chargé trouvé.\n", next_block);
            return ERROR;
        }

        // On enlève les espaces au début
        char *line = buffer;
        while (*line == ' ' || *line == '\t') line++;

        if (sscanf(line, "id : %d", &next_id) == 1) {
            continue;
        }
    }

    // Et enfin on cherche la grille
    if (Search_Key_Word(fp, grid_key)) {
        Error("Format du fichier invalide, %s introuvable.", grid_key);
        return ERROR;
    }

    // On extrait la grille
    for (int y_index = 0; y_index < GAME_API_HEIGHT; y_index++) {
        if (!fgets(buffer, BUFFER_DEBUG, fp)) {
            Error("Format du fichier invalide, %s trouvé, mais aucune données à charger.\n", grid_key);
            return ERROR;
        }
    
        char *ptr = buffer;
        char *end;
        for (int x_index = 0; x_index < GAME_API_WEIGHT; x_index++) {
            int value = (int)strtol(ptr, &end, 10);
    
            if (ptr == end) {
                Error("Valeur invalide à la ligne %d, colonne %d.\n", y_index + 1, x_index + 1);
                return ERROR;
            }
    
            grid[y_index][x_index] = value;
            ptr = end;
        }
    }

    // On set l'API avec ces valeurs
    game->pos.x = x; game->pos.y = y;

    int size = Get_Block_Size(game->id_block);
    game->id_block = current_id;
    if (Set_Block(game, IS_BLOCK, size)) return ERROR;

    size = Get_Block_Size(game->id_next_block);
    game->id_next_block = next_id;
    if (Set_Block(game, IS_NEXT_BLOCK, size)) return ERROR;

    memcpy(game->grid, grid, sizeof(game->grid));
    Display_Grid("On a cette grille :", game);

    return SUCCESS;
}

void Cancel_Rotate(APIGame *game) {
    if (!game || !game->block) {
        return;
    }

    // Vérifier si c'est le bloc O (id correspondant)
    if (game->id_block == 4) {
        return; // Ne rien faire pour BLOCK_O
    }

    int size = Get_Block_Size(game->id_block);
    int **new_block = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        new_block[i] = (int *)malloc(size * sizeof(int));
    }

    // Rotation 90° anti-horaire
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            new_block[size - 1 - j][i] = game->block[i][j];
        }
    }

    // Libération de l'ancien bloc
    for (int i = 0; i < size; i++) {
        free(game->block[i]);
    }
    free(game->block);

    // Assignation du bloc tourné
    game->block = new_block;
}

void Rotate_Block(APIGame *game) {
    if (!game || !game->block) {
        return;
    }

    // Vérifier si c'est le bloc O (id correspondant)
    if (game->id_block == 4) {
        return; // Ne rien faire pour BLOCK_O
    }

    int size = Get_Block_Size(game->id_block);
    int **new_block = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        new_block[i] = (int *)malloc(size * sizeof(int));
    }

    // Rotation 90° horaire
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            new_block[j][size - 1 - i] = game->block[i][j];
        }
    }

    // Libération de l'ancien bloc
    for (int i = 0; i < size; i++) {
        free(game->block[i]);
    }
    free(game->block);

    // Assignation du bloc tourné
    game->block = new_block;
}

int Get_X_Of_Block(APIGame *game, const int first) {
    // Permet de savoir la ligne à laquelle le bloc commence/termine en fonction du paramètre
    int size = Get_Block_Size(game->id_block);
    int start = first ? 0 : size - 1;
    int end = first ? size : -1;
    int step = first ? 1 : -1;

    for (int i = start; i != end; i += step) {
        for (int j = 0; j < size; j++) {
            if (game->block[i][j]) {
                return i;
            }
        }
    }
    return ERROR;
}

int Get_Block_Width(APIGame *game) {
    int size = Get_Block_Size(game->id_block);
    int min_col = size, max_col = 0;
    int found = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (game->block[i][j]) {
                if (!found) {
                    min_col = j;
                    found = 1;
                }
                if (j < min_col) min_col = j;
                if (j > max_col) max_col = j;
            }
        }
    }

    return (found ? max_col - min_col + 1 : 0);
}

int Is_Colision(APIGame *game) {
    int size = Get_Block_Size(game->id_block);
    Debug("la taille c'est : %d\n", size);
    // On prend le y en inverser pour évité la détection du propre bloc comme étant un autre bloc
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // On regarde si je suis sur un bloc non vide
            if (game->block[y][x]) {
                Debug("On a game->block[%d][%d] = %d\n", y, x, game->block[y][x]);
                // On se remet dans l'environement de la grille et on regarde si il y a un bloc en dessous
                Debug("Et game->grid[%d][%d] = %d\n", game->pos.y + y, game->pos.x + x, game->grid[game->pos.y + y][game->pos.x + x]);
                if (game->grid[game->pos.y + y][game->pos.x + x]) {
                    DebugSimple("\n");
                    return game->grid[game->pos.y + y][game->pos.x + x] == 9 ? COLISION_WALL : COLISION;
                }
                DebugSimple("\n");
            }
        }
    }
    DebugSimple("\n");
    return SUCCESS;
}

int Block_Physics(APIGame *game) {
    // Vérification de si il y a un bloc en dessous ou une bordure en dessous
    Display_Block("On check :", game);
    int ret = Is_Colision(game);
    Debug("On détecte %d\n", ret);
    Debug("la %d - %d\n", Get_End_Of_Block(game), game->pos.y);
    if (ret) {
        Debug("Colision détecté.\n");
        if (game->pos.y + Get_End_Of_Block(game) >= GAME_API_HEIGHT - 1) {
            return COLISION;
        } else if (game->direction != GO_DOWN) {
            return COLISION_NEUTRAL;
        }
        return ret;
    }
    return SUCCESS;
}

int Set_Block(APIGame *game, int block, const int old_size) {
    if (block == IS_BLOCK) {
        Debug("block id : %d\n", game->id_block);
        // On réalloue une nouvelle taille et on copie le bon bloc, pour le block actuelle
        int size = Get_Block_Size(game->id_block);
        game->block = realloc(game->block, size * sizeof(int *));
        if (!game->block) {
            Error("N'as pas réussis à allouer de la mémoire.\n");
            return ERROR;
        }

        // On initialise tout
        if (size > old_size) {
            for (int i = old_size; i < size; i++) {
                game->block[i] = NULL;
            }
        }        

        Debug("taille %d\n", size);
        for (int i = 0; i < size; i++) {
            Debug("game->block[%d] passe\n", i);
            game->block[i] = realloc(game->block[i], size * sizeof(int));
            if (!game->block[i]) {
                Error("N'as pas réussis à allouer de la mémoire.\n");
                return ERROR;
            }
            memcpy(game->block[i], ((int (*)[size]) shapes[game->id_block])[i], size * sizeof(int));
            Debug("fini\n");
        }
        Display_Block("On a realloc pour avoir :", game);
    } else {
        Debug("block id : %d\n", game->id_block);
        // On réalloue une nouvelle taille et on copie le bon bloc, pour le block suivant        size = Get_Block_Size(random_block);
        int size = Get_Block_Size(game->id_next_block);
        game->next_block = realloc(game->next_block, size * sizeof(int *));
        if (!game->next_block) {
            Error("N'as pas réussis à allouer de la mémoire.\n");
            return ERROR;
        }

        // On initialise tout
        if (size > old_size) {
            for (int i = old_size; i < size; i++) {
                game->next_block[i] = NULL;
            }
        }

        for (int i = 0; i < size; i++) {
            game->next_block[i] = realloc(game->next_block[i], size * sizeof(int));
            if (!game->next_block[i]) {
                Error("N'as pas réussis à allouer de la mémoire.\n");
                return ERROR;
            }

            memcpy(game->next_block[i], ((int (*)[size]) shapes[game->id_next_block])[i], size * sizeof(int));
        }
        Display_Next_Block("On a realloc pour avoir :", game);
    }
    return SUCCESS;
}

int Spawn(APIGame *game) {
    srand(time(NULL));
    // On génere un bloc random à une position random
    int random_block = (rand() % (BLOCK_COUNT - 1)) + 1;
    //random_block = 4;

    game->pos.x = 4; // Dans un tetris le bloc spawn à la 3ème colonne (1 pour la bordure)
    game->pos.y = 1;

    // On vide le blocs
    int size = Get_Block_Size(game->id_block);
    game->id_block = 0;
    Debug("On vide l'actuelle\n");
    if (Set_Block(game, IS_BLOCK, size)) return ERROR;

    Debug("On rempli l'actuelle\n");
    size = Get_Block_Size(game->id_block);
    // Sinon on met la valeur suivante dans la valeur courante
    game->id_block = game->flag ? random_block : game->id_next_block;
    if (game->flag) game->flag = !game->flag;
    if (Set_Block(game, IS_BLOCK, size)) return ERROR;

    Debug("On vide le suivant\n");
    // On vide le bloc
    size = Get_Block_Size(game->id_next_block);
    game->id_next_block = 0;
    if (Set_Block(game, IS_NEXT_BLOCK, size)) return ERROR;

    Debug("On rempli le suivant");
    // Et on mets une valeur random dans le suivant
    size = Get_Block_Size(game->id_next_block);
    random_block = (rand() % (BLOCK_COUNT - 1)) + 1;
    game->id_next_block = random_block;
    if (Set_Block(game, IS_NEXT_BLOCK, size)) return ERROR;

    return SUCCESS;
}

int Start_Game(APIGame *game) {
    game->pos.x = 0;
    game->pos.y = 0;
    game->direction = 0;
    game->flag = 1;

    game->id_block = 0;
    int size = Get_Block_Size(game->id_block);
    // Allocation du tableau de pointeurs
    game->block = malloc(size * sizeof(int *));
    if (!game->block) {
        Error("N'as pas réussi à allouer game->block.\n");
        return ERROR;
    }

    // Allocation de chaque ligne
    for (int i = 0; i < size; i++) {
        game->block[i] = malloc(size * sizeof(int));
        if (!game->block[i]) {
            Error("N'as pas réussi à allouer game->block[%d].\n", i);
            return ERROR;
        }

        // Tu peux aussi initialiser à 0 ou à partir de shapes[game->id_block]
        memset(game->block[i], 0, size * sizeof(int));
    }
    
    game->id_next_block = 0;
    size = Get_Block_Size(game->id_block);
    // Allocation du tableau de pointeurs
    game->next_block = malloc(size * sizeof(int *));
    if (!game->next_block) {
        Error("N'as pas réussi à allouer game->block.\n");
        return ERROR;
    }

    // Allocation de chaque ligne
    for (int i = 0; i < size; i++) {
        game->next_block[i] = malloc(size * sizeof(int));
        if (!game->next_block[i]) {
            Error("N'as pas réussi à allouer game->next_block[%d].\n", i);
            return ERROR;
        }

        // Tu peux aussi initialiser à 0 ou à partir de shapes[game->id_block]
        memset(game->next_block[i], 0, size * sizeof(int));
    }
    
    for (int y = 0; y < GAME_API_HEIGHT; y++) {
        for (int x = 0; x < GAME_API_WEIGHT; x++) {
            if (y == 0 || x == 0 || y == GAME_API_HEIGHT - 1 || x == GAME_API_WEIGHT - 1) {
                game->grid[y][x] = APIGAME_WALL;
            } else {
                game->grid[y][x] = 0;
            }
        }
    }
    return SUCCESS;
}

void Stop_Game(APIGame *game) {
    // On libère la mémoire alloué
    int size = Get_Block_Size(game->id_block);
    for (int i = 0; i < size; i++) {
        free(game->block[i]);
    }
    free(game->block);

    size = Get_Block_Size(game->id_next_block);
    for (int i = 0; i < size; i++) {
        free(game->next_block[i]);
    }
    free(game->next_block);
}