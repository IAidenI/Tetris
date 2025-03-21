#include "../headers/game.h"

void Display_Block(const char *text, APIGame *game) {
    Debug("%s\n", text);
    Debug("id du bloc : %d\n", game->id_block);
    int size = Get_Block_Size(game->id_block);
    for (int i = 0; i < size; i++) {
        Debug();
        for (int j = 0; j < size; j++) {
            fprintf(stderr, "%d ", game->block[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}

void Display_Next_Block(const char *text, APIGame *game) {
    Debug("%s\n", text);
    Debug("id du bloc : %d\n", game->id_next_block);
    int size = Get_Block_Size(game->id_next_block);
    for (int i = 0; i < size; i++) {
        Debug();
        for (int j = 0; j < size; j++) {
            fprintf(stderr, "%d ", game->next_block[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}

void Display_Grid(const char *text, APIGame *game) {
    Debug("%s\n", text);
    for (int i = 0; i < GAME_API_HEIGHT; i++) {
        Debug();
        for (int j = 0; j < GAME_API_WEIGHT; j++) {
            fprintf(stderr, "%d ", game->grid[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
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
    Debug("la taille c'est : %d\n", size)
    // On prend le y en inverser pour évité la détection du propre bloc comme étant un autre bloc
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // On regarde si je suis sur un bloc non vide
            if (game->block[y][x]) {
                Debug("\nOn a game->block[%d][%d] = %d\n", y, x, game->block[y][x]);
                // On se remet dans l'environement de la grille et on regarde si il y a un bloc en dessous
                Debug("Et game->grid[%d][%d] = %d\n", game->pos.y + y, game->pos.x + x, game->grid[game->pos.y + y][game->pos.x + x]);
                if (game->grid[game->pos.y + y][game->pos.x + x]) {
                    fprintf(stderr, "\n");
                    return game->grid[game->pos.y + y][game->pos.x + x] == 9 ? COLISION_WALL : COLISION;
                }
                fprintf(stderr, "\n");
            }
        }
    }
    fprintf(stderr, "\n");
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
        return game->pos.y + Get_End_Of_Block(game) >= 21 ? COLISION : ret;
    }
    return SUCCESS;
}

int Set_Block(APIGame *game, int block) {
    if (block == IS_BLOCK) {
        // On réalloue une nouvelle taille et on copie le bon bloc, pour le block actuelle
        int size = Get_Block_Size(game->id_block);
        game->block = realloc(game->block, size * sizeof(int *));
        if (!game->block) {
            Error("N'as pas réussis à allouer de la mémoire.\n");
            return ERROR;
        }

        for (int i = 0; i < size; i++) {
            game->block[i] = realloc(game->block[i], size * sizeof(int));
            if (!game->block[i]) {
                Error("N'as pas réussis à allouer de la mémoire.\n");
                return ERROR;
            }
            memcpy(game->block[i], ((int (*)[size]) shapes[game->id_block])[i], size * sizeof(int));
        }
        Display_Block("On a realloc pour avoir :", game);
    } else {
        // On réalloue une nouvelle taille et on copie le bon bloc, pour le block suivant        size = Get_Block_Size(random_block);
        int size = Get_Block_Size(game->id_next_block);
        game->next_block = realloc(game->next_block, size * sizeof(int *));
        if (!game->next_block) {
            Error("N'as pas réussis à allouer de la mémoire.\n");
            return ERROR;
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

    void **shapes = Get_Shapes();
    int size = Get_Block_Size(random_block);

    // Si on est au début donc pas de bloc initialisé
    if (game->block == NULL) {
        // On affecte le random au bloc courant
        game->id_block = random_block;
        game->block = malloc(size * sizeof(int *));
        if (!game->block) {
            Error("N'as pas réussis à allouer de la mémoire.\n");
            return ERROR;
        }

        for (int i = 0; i < size; i++) {
            game->block[i] = malloc(size * sizeof(int));
            if (!game->block[i]) {
                Error("N'as pas réussis à allouer de la mémoire.\n");
                return ERROR;
            }
            memcpy(game->block[i], ((int (*)[size]) shapes[game->id_block])[i], size * sizeof(int));
        }

        // Et on fait un autre random pour le next
        random_block = (rand() % (BLOCK_COUNT - 1)) + 1;
        //random_block = 4;
        size = Get_Block_Size(random_block);
        
        game->id_next_block = random_block;
        game->next_block = malloc(size * sizeof(int *));
        if (!game->next_block) {
            Error("N'as pas réussis à allouer de la mémoire.\n");
            return ERROR;
        }

        for (int i = 0; i < size; i++) {
            game->next_block[i] = malloc(size * sizeof(int));
            if (!game->next_block[i]) {
                Error("N'as pas réussis à allouer de la mémoire.\n");
                return ERROR;
            }
            memcpy(game->next_block[i], ((int (*)[size]) shapes[game->id_next_block])[i], size * sizeof(int));
        }
    } else {
        // On vide le blocs
        game->id_block = 0;
        Debug("On vide\n");
        if (Set_Block(game, IS_BLOCK)) return ERROR;

        Debug("On rempli\n");
        // Sinon on mets la valeur suivante dans la valeur courante
        game->id_block = game->id_next_block;
        if (Set_Block(game, IS_BLOCK)) return ERROR;

        // On vide le bloc
        game->id_next_block = 0;
        if (Set_Block(game, IS_NEXT_BLOCK)) return ERROR;

        // Et on mets le random dans le suivant
        game->id_next_block = random_block;
        if (Set_Block(game, IS_NEXT_BLOCK)) return ERROR;
    }
    return SUCCESS;
}

int Start_Game(APIGame *game) {
    game->pos.x = 0;
    game->pos.y = 0;
    game->block = NULL;
    game->next_block = NULL;
    game->grid = malloc((GAME_API_HEIGHT) * sizeof(int *));
    if (!game->grid) {
        Error("N'as pas réussis à allouer de la mémoire.\n");
        return ERROR;
    }

    // On initialise toutes les lignes avec des ' '
    for (int i = 0; i < GAME_API_HEIGHT; i++) {
        game->grid[i] = malloc((GAME_API_WEIGHT) * sizeof(int));
        if (!game->grid[i]) {
            Error("N'as pas réussis à allouer de la mémoire.");
            return ERROR;
        }
        // On ajoute le contenu
        for (int j = 0; j < GAME_API_WEIGHT; j++) {
            // Mise en place du contour
            if (i == 0 || j == 0 || i == GAME_API_HEIGHT - 1 || j == GAME_API_WEIGHT - 1) {
                game->grid[i][j] = APIGAME_WALL;
            } else {
                game->grid[i][j] = 0;
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

    for (int i = 0; i < GAME_API_HEIGHT; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
}