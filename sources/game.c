#include "../headers/game.h"

/*
    Game
*/
int Start_Game(APIGame *game) {
    game->pos.x = 0;
    game->pos.y = 0;
    game->direction = 0;

    game->state.score = 0;
    game->state.level = 1;
    game->state.nb_lines = 0;
    game->rotation = 0;
    if (Compute_Gravity(&game->state)) {
        return ERROR;
    }

    game->flag = FLAG_START;
    Fill_Bag(game);
    Display_Bag("Bag initialisé :", game);

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
        for (int x = 0; x < GAME_API_WIDTH; x++) {
            if (y == 0 || x == 0 || y == GAME_API_HEIGHT - 1 || x == GAME_API_WIDTH - 1) {
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



/*
    Relatif à la création d'un bloc
*/
int Spawn(APIGame *game) {
    // On génere un bloc random en utilisant la méthode 7-bag
    int random_block = Random_Block(game);
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
    Debug("flag : %d\n", game->flag);
    game->id_block = game->flag == FLAG_START ? random_block : game->id_next_block;
    if (Set_Block(game, IS_BLOCK, size)) return ERROR;

    Debug("On vide le suivant\n");
    // On vide le bloc
    size = Get_Block_Size(game->id_next_block);
    game->id_next_block = 0;
    if (Set_Block(game, IS_NEXT_BLOCK, size)) return ERROR;

    Debug("On rempli le suivant\n");
    // Et on mets une valeur random dans le suivant
    size = Get_Block_Size(game->id_next_block);
    random_block = game->flag == FLAG_START ? Random_Block(game) : random_block;
    if (game->flag == FLAG_START) game->flag = FLAG_NEUTRAL;
    game->id_next_block = random_block;
    if (Set_Block(game, IS_NEXT_BLOCK, size)) return ERROR;

    return SUCCESS;
}

int Random_Block(APIGame *game) {
    srand(time(NULL));
    // Si le sac est vide on le rempli
    if (Is_Bag_Empty(game)) Fill_Bag(game);
    Display_Bag("Bag :", game);
    
    // On récupère les indices des blocs encore présent
    int available_indices[BLOCK_COUNT - 1];
    int count = 0;
    for (int i = 0; i < BLOCK_COUNT - 1; i ++) {
        if (game->seven_bag[i] != 0) {
            available_indices[count++] = i;
        }
    }

    // On choisi un nombre random parmis les restant
    int random_pos = rand() % count;
    int random_block = game->seven_bag[available_indices[random_pos]];
    game->seven_bag[available_indices[random_pos]] = 0;

    Debug("random : %d\n", random_block);
    Display_Bag("Nouveau bag :", game);
    return random_block;
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



/*
    Relatif au colisions
*/
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




/*
    Relatif aux rotations de blocs
*/
void Rotate_Block(APIGame *game) {
    if (!game || !game->block) {
        return;
    }

    // Vérifier si c'est le bloc O (id correspondant)
    if (game->id_block == 4) {
        return; // Ne rien faire pour BLOCK_O
    }

    Display_Block("Avant :", game);
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
    Display_Block("Après :", game);
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




/*
    Récuperation d'information sur un bloc
*/
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




/*
    Relatif au scoring
*/
const char *Get_Difficulty(int level) {
    if (level >= 0 && level <= 4) {
        return "Easy";
    } else if (level >= 5 && level <= 9) {
        return "Medium";
    } else if (level >= 10 && level <= 15) {
        return "Hard";
    } else if (level >= 16 && level <= 28) {
        return "Expert";
    } else if (level >= 29) {
        return "Insane";
    }

    return "Inconnu";
}

void Compute_Score(GameState *state, int lines) {
    Debug("aaaa : %d\n", lines);
    static const int base_scores[] = {0, 40, 100, 300, 1200};
    if (lines >= 1 && lines <= 4) {
        Debug("Donc : %d * %d\n", base_scores[lines], state->level);
        state->score += base_scores[lines] * state->level;
    }
    Debug("Modif : %d\n", state->score);
}

int Compute_Gravity(GameState *state) {
    const int table_gravity[] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 4, 3, 2, 1};
    int frames = 0;

    if (state->level == 0) {
        frames = table_gravity[state->level];
    } else if (state->level >= 1 && state->level <= 8) {
        frames = table_gravity[state->level - 1];
    } else if (state->level == 9) {
        frames = table_gravity[9];
    } else if (state->level >= 10 && state->level <= 12) {
        frames = table_gravity[10];
    } else if (state->level >= 13 && state->level <= 15) {
        frames = table_gravity[11];
    } else if (state->level >= 16 && state->level <= 18) {
        frames = table_gravity[12];
    } else if (state->level >= 19 && state->level <= 28) {
        frames = table_gravity[13];
    } else if (state->level >= 29) {
        frames = table_gravity[14];
    } else {
        Error("Impossible de calculer la gravité.\n");
        return ERROR;
    }

    // On calcule la conversion en ms
    state->speed = (frames / 60.0) * 1000.0;
    Debug("On a comme nouvelle speed : %f ms\n", state->speed);

    return SUCCESS;
}



/*
    Relatif à au 7-bag
*/
void Fill_Bag(APIGame *game) {
    for (int i = 0; i < BLOCK_COUNT - 1; i++) {
        game->seven_bag[i] = i + 1;
    }
}

int Is_Bag_Empty(APIGame *game) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (game->seven_bag[i] != 0) return 0;
    }
    return 1;
}



/*
    Relatif au snapshot
*/
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
    const char *rotation = "[rotation]";
    const char *grid_key = "[grid]";

    char buffer[BUFFER_DEBUG];
    int x = -1, y = -1, current_id = -1, next_id = -1, rot = -1;
    int grid[GAME_API_HEIGHT][GAME_API_WIDTH];

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


    // On cherche les rotations du bloc
    if (Search_Key_Word(fp, rotation)) {
        Error("Format du fichier invalide, %s introuvable.\n", rotation);
        return ERROR;
    }

    // On extrait les rotations
    while (rot == -1) {
        if (!fgets(buffer, BUFFER_DEBUG, fp)) {
            Error("Format du fichier invalide, %s trouvé, mais aucune données à chargé trouvé.\n", rotation);
            return ERROR;
        }

        // On enlève les espaces au début
        char *line = buffer;
        while (*line == ' ' || *line == '\t') line++;

        if (sscanf(line, "rot : %d", &rot) == 1) {
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
        for (int x_index = 0; x_index < GAME_API_WIDTH; x_index++) {
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

    game->rotation = (rot + 1) % 4;
    Debug("%d rotations\n", game->rotation);
    for (int i = 0; i < game->rotation; i ++) {
        Rotate_Block(game);
    }

    memcpy(game->grid, grid, sizeof(game->grid));
    Display_Grid("On a cette grille :", game);

    return SUCCESS;
}


void Snapshot_Gen(APIGame *game) {
    // Liste des key words
    const char *position = "[position]";
    const char *current_block = "[current block]";
    const char *next_block = "[next block]";
    const char *rotation = "[rotation]";
    const char *grid_key = "[grid]";

    FILE *fp = fopen(SNAPSHOT_FILE, "w");

    // On commencer par écrire la position du bloc actuelle dans le fichier
    fprintf(fp, "%s\n", position);
    fprintf(fp, "    x : %d\n", game->pos.x);
    fprintf(fp, "    y : %d\n\n", game->pos.y);

    // On cherche ensuite l'id du bloc actuelle
    fprintf(fp, "%s\n", current_block);
    fprintf(fp, "    id : %d\n\n", game->id_block);

    // On écrit l'id du bloc suivant
    fprintf(fp, "%s\n", next_block);
    fprintf(fp, "    id : %d\n\n", game->id_next_block);

    // On écrit les rotations du bloc
    fprintf(fp, "%s\n", rotation);
    fprintf(fp, "    rot : %d\n\n", game->rotation);

    // Et enfin on écrit la grille
    fprintf(fp, "%s\n", grid_key);
    for (int y = 0; y < GAME_API_HEIGHT; y++) {
        fprintf(fp, "    ");
        for (int x = 0; x < GAME_API_WIDTH; x++) {
            fprintf(fp, "%d ", game->grid[y][x]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    fclose(fp);
}



/*
    Relatif à la linge full
*/
int Clear_Full_Lines(APIGame *game) {
    // On vérifie si une ligne est pleine
    int full_cases = 0;
    int full_count = 0;
    int *full_lines = NULL;

    Debug("pos y : %d - max : %d\n", game->pos.y, game->pos.y + Get_End_Of_Block(game));
    for (int y = game->pos.y; y < GAME_API_HEIGHT - 1 && y <= game->pos.y + Get_End_Of_Block(game); y++) {
        for (int x = 1; x < GAME_API_WIDTH - 1; x++) {
            // Si un bloc est présent un incrémente notre compteur
            Debug("On test : x : %d - y : %d\n", x, y);
            if (game->grid[y][x]) {
                Debug("Oui %d\n", game->grid[y][x]);
                full_cases++;
            }
        }
        if (full_cases == GAME_API_WIDTH - 2) {
            full_lines = realloc(full_lines, (full_count + 1) * sizeof(int));
            if (!full_lines) {
                Error("N'as pas réussis à réallouer de la mémoire.\n");
                return ERROR;
            }
            full_lines[full_count++] = y;
        }
        full_cases = 0;
    }

    Debug("On a détecter %d lines complètes.\n", full_count);
    Debug("bbbb : %d\n", game->state.nb_lines);
    if (!full_count) return SUCCESS;
    
    // On met à jour le niveau
    game->state.nb_lines += full_count;
    Debug("aaa : %d\n", game->state.nb_lines);
    if (game->state.nb_lines >= 10) {
        Debug("weeeee\n");
        game->state.level++;
        game->state.nb_lines = 0;
    }

    //On met à jour le score
    Compute_Score(&game->state, full_count);
    Compute_Gravity(&game->state);

    // On supprime toutes les lignes full
    for (int i = 0; i < full_count; i++) {
        for (int x = 1; x < GAME_API_WIDTH - 1; x++) {
            Debug("test de %d\n", full_lines[i]);
            Debug("Donc %d\n", game->grid[full_lines[i]][x]);
            game->grid[full_lines[i]][x] = 0;
        }
    }

    Debug("C'est fait.\n");

    // On récupère l'index de la première ligne vide (en partant du bas)
    int max_bloc_y = 0;
    for (int y = full_lines[0] - 1; y > 0; y--) {
        for (int x = 1; x < GAME_API_WIDTH - 1; x++) {
            if (game->grid[y][x] == 0) {
                max_bloc_y = 1;
            } else {
                max_bloc_y = 0;
                break;
            }
        }
        if (max_bloc_y) {
            max_bloc_y = y;
            break;
        }
    }

    Display_Grid("grille avant :", game);
    // On descend les lignes au-dessus des lignes supprimées
    for (int i = 0; i < full_count; i++) {
        int y = full_lines[i];

        Debug("On le fait pour y = %d\n", y);
        // Déplacer toutes les lignes du dessus vers le bas
        // Ca sert à rien d'aller jusqu'à row > 1 car on a calculer l'index
        for (int row = y; row >= max_bloc_y; row--) {
            for (int x = 1; x < GAME_API_WIDTH - 1; x++) {
                // On prends pas si c'est un wall
                if (game->grid[row][x] != APIGAME_WALL) {
                    game->grid[row][x] = game->grid[row - 1][x];
                }
            }
        }
    }
    Display_Grid("grille après :", game);

    free(full_lines);
    return CLEAR_BLOCK;
}



/*
    Relatif à l'ajout d'un bloc
*/
void Update_Block_API(APIGame *game, const int x, const int y, const int value, const wchar_t *state) {
    Debug("On met à jour l'API avec game->grid[%d][%d] = %d\n", y, x, wcscmp(state, BLOCK) == 0 ? value : 0);
    game->grid[y][x] = wcscmp(state, BLOCK) == 0 ? value : 0;
}