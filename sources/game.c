#include "../headers/game.h"

int Get_X_Of_Block(APIGame *game, const int first) {
    // Permet de savoir la ligne à laquelle le bloc commence/termine en fonction du paramètre
    int start = first ? 0 : BLOCK_SIZE - 1;
    int end = first ? BLOCK_SIZE : -1;
    int step = first ? 1 : -1;

    for (int i = start; i != end; i += step) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (game->block[i][j]) {
                return i;
            }
        }
    }
    return ERROR;
}

int Get_Block_Width(APIGame *game) {
    int min_col = BLOCK_SIZE, max_col = 0;
    int found = 0;

    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
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
    // On prend le y en inverser pour évité la détection du propre bloc comme étant un autre bloc
    int api_x = game->pos.x / GAME_WEIGHT_MUL;
    int api_y = game->pos.y;
    int offset = game->pos.x >= 0 ? 1 : -1;
    for (int y = 0; y < BLOCK_SIZE; y++) {
        for (int x = 0; x < BLOCK_SIZE; x++) {
            // On regarde si je suis sur un bloc non vide
            if (game->block[y][x]) {
                Debug("On a game->block[%d][%d] = %d\n", y, x, game->block[y + 1][x]);
                // On se remet dans l'environement de la grille et on regarde si il y a un bloc en dessous
                Debug("Et game->grid[%d][%d] = %d\n", api_y + y, api_x + x + offset, game->grid[api_y + y][api_x + x + offset]);
                if (game->grid[api_y + y][api_x + x + offset] && !game->block[y + 1][x]) { // Ici il va y avoir un problème avec les rotation à cause du y + 1
                    fprintf(stderr, "\n");
                    return game->grid[api_y + y][api_x + x + offset] == 9 ? COLISION_WALL : COLISION;
                }
            }
        }
    }
    fprintf(stderr, "\n");
    return SUCCESS;
}

int Block_Physics(APIGame *game) {
    // Vérification de si il y a un bloc en dessous ou une bordure en dessous
    int ret = Is_Colision(game);
    Debug("On détecte %d\n", ret);
    Debug("la %d - %d\n", Get_End_Of_Block(game), game->pos.y);
    if (ret) {
        if (game->pos.y == 1) {
            Debug("Perdu!\n");
            return LOOSE;
        } else if (game->pos.y + Get_End_Of_Block(game) >= 21) {
            return COLISION;
        }
        Debug("Colision détecté.\n");
        return ret;
    }
    return SUCCESS;
}

void Spawn(APIGame *game) {
    srand(time(NULL));
    // On génere un bloc random à une position random
    int random_bloc = (rand() % (BLOCK_COUNT - 1)) + 1;
    
    game->pos.x = 7; // Dans un tetris le bloc spawn à la 3ème colonne (1 pour la bordure 3 * 2 car une case c'est 2 caractères
    game->pos.y = 1;

    int (**shapes)[BLOCK_SIZE] = Get_Shapes();

    // Si on est au début donc pas de bloc initialisé
    if (game->block == NULL) {
        // On affecte le random au bloc courant
        game->block = shapes[random_bloc];
        game->id_block = random_bloc;

        // Et on fait un autre random pour le next
        random_bloc = (rand() % (BLOCK_COUNT - 1)) + 1;
        game->next_block = shapes[random_bloc];
        game->id_next_block = random_bloc;
    } else {
        // On vide les blocs
        game->block = shapes[0];
        game->next_block = shapes[0];

        // Sinon on mets la valeur suivante dans la valeur courante
        game->block = shapes[game->id_next_block];
        game->id_block = game->id_next_block;

        // Et on mets le random dans le suivant
        game->next_block = shapes[random_bloc];
        game->id_next_block = random_bloc;
    }
}

int Start_Game(APIGame *game) {
    game->pos.x = 0;
    game->pos.y = 0;
    game->block = NULL;
    game->next_block = NULL;
    game->grid = malloc((GAME_API_HEIGHT) * sizeof(int *));
    if (!game->grid) {
        Error("N'as pas réussis à allouer de la mémoire.");
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
    for (int i = 0; i < GAME_API_HEIGHT; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
}