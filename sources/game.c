#include "../headers/game.h"

/* Gestion du cardre de la game */
Block block[] = {
    {BLOCK_I, LIGHT_BLUE},
    {BLOCK_J, DARK_BLUE},
    {BLOCK_L, ORANGE},
    {BLOCK_O, YELLOW},
    {BLOCK_S, GREEN},
    {BLOCK_T, PURPLE},
    {BLOCK_Z, RED},
};

void Loose(APIGame *game) {
    // On fait clignoter le bloc qui pose problème
    Debug("pos (x : %d - y : %d)\n", game->pos.x, game->pos.y);
    Debug("shape :\n%ls\n", game->block.shape);
    for (int i = 0; i < 3; i++) {
        Debug("Deleting...\n");
        Del_Block(game);
            // Pour le debug
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        Debug("%ls", game->lines[i]);
    }
        Debug("Waiting...\n\n");
        napms(1000);
        Debug("Adding...\n");
        Put_Block(game);
            // Pour le debug
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        Debug("%ls", game->lines[i]);
    }
        Debug("Done.\n\n");
    }
}

int Get_True_Pos(APIGame *game) {
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    int line = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token != NULL) {
        if (wcsstr(token, BLOCK_CHAR) != NULL) {
            return game->pos.y + line;
        }
        token = wcstok(NULL, L"\n", &saved);
        line++;
    }
    return 0;
}

size_t Block_Size(const wchar_t *shape) {
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    size_t count = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);

    while (token != NULL) {
        if (wcsstr(token, BLOCK_CHAR) != NULL) { // On ne compte que les lignes contenant []
            count++;
        }
        token = wcstok(NULL, L"\n", &saved);
    }

    return count;
}

size_t Block_Max_Lenth(const int i) {
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, block[i].shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    size_t max_length = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);

    while (token != NULL) {
        if (wcsstr(token, BLOCK_CHAR) != NULL) { // On ne prend en compte que les lignes avec []
            size_t length = wcslen(token);
            if (length > max_length) {
                max_length = length;
            }
        }
        token = wcstok(NULL, L"\n", &saved);
    }
    return max_length;
}

int Block_Physics(APIGame *game) {
    int block_size = (int)Block_Size(game->block.shape);
    int true_y = Get_True_Pos(game);
    // Si on est un bloc avant la bordure
    if (true_y >= GAME_HEIGHT) {
        return ERROR;
    }

    // On ajoute la taille du bloc en hauteur pour pouvoir "voir" en dessous
    int true_y_back = true_y;
    true_y += block_size - 1;

    for (int i = 0; game->lines[true_y][i] != L'\0' && game->lines[true_y + 1][i] != L'\0'; i++) {
        if (game->lines[true_y][i] == L'[' && game->lines[true_y][i + 1] == L']' && game->lines[true_y + 1][i] == L'[' && game->lines[true_y + 1][i + 1] == L']') {
            Debug("DETECTE1 : '%lc'\n", game->lines[true_y][i]);
            Debug("DETECTE2 : '%lc'\n", game->lines[true_y + 1][i]);
            Debug("y : %d - i : %d\n", true_y, i);
                    Debug("On vérifie la ligne (%d) :\n%ls", true_y, game->lines[true_y] + i);
        Debug("Avec la ligne (%d) :\n%ls", true_y + 1, game->lines[true_y + 1] + i);
            // On vérifie si c'est loose
            if (true_y_back == 1) {
                Debug("Loose détecté.\n");
                Loose(game);
                return LOOSE;
            } else {
                return ERROR;
            }
        }
    }
    return SUCCESS;
}

void Put_Next_Block(APIGame *game) {
    // On calcul la positon pour le bloc
    int x = (int)(GAME_WEIGHT + 2 + ((NEXT_BLOCK_WEIGHT + 2) / 2 - Block_Max_Lenth(game->id_next_block) / 2));
    int y = NEXT_BLOCK_HEIGHT - 1;

    // On vide l'emplacement
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < NEXT_BLOCK_WEIGHT; j++) {
            wchar_t clear_token[NEXT_BLOCK_WEIGHT];
            wmemset(clear_token, L' ', NEXT_BLOCK_WEIGHT);
            clear_token[NEXT_BLOCK_WEIGHT - 1] = L'\0';
            mvaddwstr(y + i, GAME_WEIGHT + 3, clear_token);
        }
    }

    if (game->next_block.shape != NULL) {
        // strtok trop chiant
        wchar_t buffer[MAX_COPY];
        wmemset(buffer, L'\0', MAX_COPY);
        wcsncpy(buffer, game->next_block.shape, MAX_COPY - 1);
        buffer[MAX_COPY - 1] = L'\0';

        // On ajoute le prochain bloc
        int i = 0;
        wchar_t *saved;
        wchar_t *token = wcstok(buffer, L"\n", &saved);
        while (token != NULL) {
            // On affiche juste les caratères []
            if (wcsstr(token, BLOCK_CHAR) != NULL) {
                //Debug("token %ls\n", token);
                attron(COLOR_PAIR(game->next_block.color));
                mvaddwstr(y + i, x, token); // Pour le mettre au milieu
                attroff(COLOR_PAIR(game->next_block.color));
                refresh();
                token = wcstok(NULL, L"\n", &saved);
                i++;
            } else {
                token = wcstok(NULL, L"\n", &saved);
                i++;
            }
        }

        //Debug("next pos (x : %d - y : %d)\n", x, y);
    }
}

void Put_Block(APIGame *game) {
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    int lineIndex = 0;         // Compteur de ligne
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);

    while (token != NULL) {
        // Longueur de la ligne
        int len = wcslen(token);

        // Parcours de la ligne
        for (int j = 0; j < len - 1; j++) {
            // On cherche un '[' suivi de ']'
            if (token[j] == L'[' && token[j + 1] == L']') {
                // Écrire "[]" dans la mémoire (si nécessaire pour ton plateau)
                Debug("On mets [] à la position x : %d - y : %d\n", game->pos.x + j, game->pos.y + lineIndex);
                wmemcpy(game->lines[game->pos.y + lineIndex] 
                        + (game->pos.x + j), BLOCK_CHAR, 2);

                // Affichage du bloc en couleurs
                attron(COLOR_PAIR(game->block.color));
                mvaddwstr(game->pos.y + lineIndex, 
                          game->pos.x + j, 
                          BLOCK_CHAR);
                attroff(COLOR_PAIR(game->block.color));
                refresh();

                // On saute le caractère ']' pour éviter de le reprendre
                j++;
            }
        }

        token = wcstok(NULL, L"\n", &saved);
        lineIndex++;
    }
}

void Del_Block(APIGame *game) {
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    int lineIndex = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);

    while (token != NULL) {
        int len = wcslen(token);

        // On parcourt la ligne du bloc pour supprimer uniquement les "[]"
        for (int j = 0; j < len - 1; j++) {
            if (token[j] == L'[' && token[j + 1] == L']') {
                Debug("Suppression de [] à la position x : %d - y : %d\n", 
                      game->pos.x + j, game->pos.y + lineIndex);

                // Remplace uniquement les "[]" par des espaces dans la mémoire
                wmemcpy(game->lines[game->pos.y + lineIndex] + (game->pos.x + j), L"  ", 2);

                // Met à jour l'affichage
                wchar_t clear_token[3] = {L' ', L' ', L'\0'};
                mvaddwstr(game->pos.y + lineIndex, game->pos.x + j, clear_token);
                refresh();

                // On saute le caractère ']' pour ne pas l'analyser à nouveau
                j++;
            }
        }

        token = wcstok(NULL, L"\n", &saved);
        lineIndex++;
    }

    Debug("Contenu de game->lines[%d] après suppression : %ls\n", game->pos.y + lineIndex - 1, game->lines[game->pos.y + lineIndex - 1]);
}

int Place_Block(APIGame *game) {
    Debug("[ ] Informations sur le bloc :\n");
    Debug("    - pos          x : %d - y : %d)\n", game->pos.x, game->pos.y);
    Debug("    - bloc id      %d\n", game->id_block);
    Debug("    - next bloc id %d\n", game->id_next_block);
    Debug("    - shape\n%ls\n", game->block.shape);

    // On vérifie si il y a un block en dessous
    int ret = Block_Physics(game);
    if (ret) {
        return ret;
    }

    // On supprimer l'ancienne position du bloc
    Del_Block(game);

    // On descend le bloc
    game->pos.y++;
    //napms(2000);

    // Ajoute le bloc à la nouvelle position
    Put_Block(game);

    // Pour le debug
    Debug("API tetris:\n");
    Debug("next\n%ls\n", game->next_block.shape);
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        Debug("%ls", game->lines[i]);
    }
    fprintf(stderr, "\n");
    return SUCCESS;
}

void Borders(const wchar_t *c1, const wchar_t *c2) {
    addwstr(c1);
    for(int i = 0; i < GAME_WEIGHT; i++) {
        addwstr(MIDLESCORE);
    }
    addwstr(c2);
}

void Create_Frame() {
    clear();
    Top_Border();
    addwstr(CORNER_TOP_LEFT);
    for (int j = 0; j < NEXT_BLOCK_IHM_LEN; j++) {
        addwstr(MIDLESCORE);
    }
    addwstr(CORNER_TOP_RIGHT);
    addwstr(L"\n");


    for (int i = 0; i < GAME_HEIGHT; i++) {
	    addwstr(WALL);
	    for (int j = 0; j < GAME_WEIGHT; j++) {
	        addwstr(L" ");
	    }
	    addwstr(WALL);
        if (i == 0) {
            addwstr(WALL);
            addwstr(L" Next block ");
            addwstr(WALL);
        } else if (i == 1) {
            addwstr(CROSS_LEFT);
            for (int j = 0; j < NEXT_BLOCK_IHM_LEN; j++) {
                addwstr(MIDLESCORE);
            }
            addwstr(CROSS_RIGHT);
        } else if (i == 2 || i == 3) {
            addwstr(WALL);
            for (int j = 0; j < NEXT_BLOCK_IHM_LEN; j++) {
                addwstr(L" ");
            }
            addwstr(WALL);
        } else if (i == 4) {
            addwstr(CORNER_BOT_LEFT);
            for (int j = 0; j < NEXT_BLOCK_IHM_LEN; j++) {
                addwstr(MIDLESCORE);
            }
            addwstr(CORNER_BOT_RIGHT);
        }
	    addwstr(L"\n");
    }

    Bottom_Border();
    addwstr(L"\n");
}

/* Gestion de la game */
void Spawn(APIGame *game, const int id_block) {
    srand(time(NULL));

    // On génere un bloc random à une position random
    int random_bloc;
    if (id_block == -1) {
        random_bloc = rand() % (sizeof(block) / sizeof(block[0]));
    } else {
        random_bloc = id_block;
    }
    
    game->pos.x = 7; // Dans un tetris le bloc spawn à la 3ème colonne (1 pour la bordure 3 * 2 car une case c'est 2 caractères
    game->pos.y = 1;

    // Si on est au début donc pas de bloc initialisé
    if (game->block.shape == NULL) {
        // On affecte le random au bloc courant
        game->block.shape = wcsdup(block[random_bloc].shape);
        game->block.color = block[random_bloc].color;
        game->id_block = random_bloc;

        // Et on fait un autre random pour le next
        random_bloc = rand() % (sizeof(block) / sizeof(block[0]));
        game->next_block.shape = wcsdup(block[random_bloc].shape);
        game->next_block.color = block[random_bloc].color;
        game->id_next_block = random_bloc;
    } else {
        // Sinon on mets la valeur suivante dans la valeur courante
        game->block.shape = game->next_block.shape;
        game->block.color = game->next_block.color;
        game->id_block = game->id_next_block;

        // Et on mets le random dans le suivant
        game->next_block.shape = block[random_bloc].shape;
        game->next_block.color = block[random_bloc].color;
        game->id_next_block = random_bloc;
    }

    Put_Block(game);

    // Pour le debug
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        Debug("%ls", game->lines[i]);
    }
    printf("\n");
}

int Game(const char *block_choose) {
    initscr();   // Initialise ncurses
    // Vérifie que le terminal supporte les couleurs
    if (!has_colors()) {
        endwin();
        Error("Votre terminal ne supporte pas les couleurs.\n");
        return ERROR;
    }
    noecho();              // Déactive l'affiche de la saisie utilisateur (si on presse sur 'k' il ne va pas s'afficher à l'écran
    curs_set(0);           // Cache le curseur
    setlocale(LC_ALL, ""); // Active l'UTF-8
    keypad(stdscr, TRUE);  // Pour détecter les flêches
    use_default_colors();
    Init_Colors();

    int key;
    int temp = 0;
    int has_spawned = 0;

    // Création de l'API game pour pouvoir gerer correctement la victoire ou autre
    APIGame game;
    game.pos.x = 0;
    game.pos.y = 0;
    game.block.shape = NULL;
    game.block.color = block[0].color;
    game.next_block.shape = NULL;
    game.next_block.color = block[0].color;
    game.lines = malloc((GAME_HEIGHT + 2) * sizeof(wchar_t *));
    if (!game.lines) {
        Error("N'as pas réussis à allouer de la mémoire.");
        return ERROR;
    }

    // On initialise toutes les lignes avec des ' '
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        game.lines[i] = malloc((GAME_WEIGHT + 4) * sizeof(wchar_t));
        if (!game.lines[i]) {
            Error("N'as pas réussis à allouer de la mémoire.");
            return ERROR;
        }

        // On ajoute les bordures
        if (i == 0 || i == GAME_HEIGHT + 1) {
            wmemset(game.lines[i], L'-', GAME_WEIGHT + 3);
            game.lines[i][GAME_WEIGHT + 2] = L'\n';
            game.lines[i][GAME_WEIGHT + 3] = L'\0';
        } else {
            // On ajoute le contenu
            wmemset(game.lines[i], L' ', GAME_WEIGHT + 3);

            // On ajoute les bords
            game.lines[i][0] = *WALL;
            game.lines[i][GAME_WEIGHT + 1] = *WALL;
            game.lines[i][GAME_WEIGHT + 2] = L'\n';
            game.lines[i][GAME_WEIGHT + 3] = L'\0';
        }
    }

    clear();
    // On crée le cadre
    Create_Frame();
    while (1) {
        // On vérifie si un bloc à déjà spawn pour pas en faire apparaire plusieurs
        if (!has_spawned) {
            if (strcmp(block_choose, "I") == 0) {
                Spawn(&game, 0);
            } else if (strcmp(block_choose, "J") == 0) {
                Spawn(&game, 1); 
            } else if (strcmp(block_choose, "L") == 0) {
                Spawn(&game, 2);
            } else if (strcmp(block_choose, "O") == 0) {
                Spawn(&game, 3);
            } else if (strcmp(block_choose, "S") == 0) {
                Spawn(&game, 4);
            } else if (strcmp(block_choose, "T") == 0) {
                Spawn(&game, 5);
            } else if (strcmp(block_choose, "Z") == 0) {
                Spawn(&game, 6);
            } else {
                Spawn(&game, -1);
            }
            Debug("[ ] Informations spawn:\n");
            Debug("    - shape\n%ls\n", game.block.shape);
            Debug("    - next shape\n%ls\n", game.next_block.shape);
            has_spawned = 1;
        } else {
            int ret = Place_Block(&game);

            if (ret) {
                if (ret == LOOSE) {
                    return ret;
                }
                game.pos.y = 0;
                has_spawned = 0;
            }
        }
	    refresh();

        Put_Next_Block(&game);

        // On attends un peu avant la prochaine position du bloc
	    napms(BLOCK_WAIT);

        // On vérifie si il est en bas
	    size_t height = Block_Size(game.block.shape) == 1 ? GAME_HEIGHT : GAME_HEIGHT - 1;
	    //Debug("pos %d - height %ld\n", game.pos.y, height);
        if (game.pos.y >= height) {
	        key = getch(); // On récupère l'ebtré utilisateur
	        if (temp == 3) {
	            break;
	        }
	        temp++;
            game.pos.y = 0;
            has_spawned = 0;
	    }
    }

    endwin();
    // On libère la mémoire alloué
    for (int i = 0; i < GAME_HEIGHT; i++) {
        free(game.lines[i]);
    }
    free(game.lines);
    return SUCCESS;
}
