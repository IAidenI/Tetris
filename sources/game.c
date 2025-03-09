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

size_t Block_Size(const wchar_t *shape) {
    // strtok veux pas de const donc on fait une copie de la forme
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    // On compte combien il y a de lignes dans la forme
    size_t lenth = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token != NULL) {
	    token = wcstok(NULL, L"\n", &saved);
	    lenth++;
    }

    return lenth;
}

size_t Block_Max_Lenth(const int i) {
    // strtok veux pas de const donc on fait une copie de la forme
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, block[i].shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    size_t lenth = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token) {
        lenth = wcslen(token) > lenth ? wcslen(token) : lenth;
        token = wcstok(NULL, L"\n", &saved);
    }
    return lenth;
}

int Block_Physics(APIGame *game) {
    int offset = Block_Size(game->block.shape);
    Debug("offset = %d\n", offset);

    // Si le bloc touche la bordure basse
    if (game->pos.y + offset == GAME_HEIGHT) { // Si on est un bloc avant la bordure
        return 0;
    } else if (game->pos.y + offset >= GAME_HEIGHT) { // Si on est juste avant la bordure
        return 1;
    }

    // Si la case en dessous c'est un bloc
    if (wcsstr(game->lines[game->pos.y + offset], L"[]") != NULL) {
        Debug("Bloc détecté dans (%d)\n%ls", game->pos.y + 1, game->lines[game->pos.y + 1]);
        return 1;
    }
    return 0;
}

void Put_Next_Block(APIGame *game) {
    // On calcul la positon pour le bloc
    int x = (int)GAME_WEIGHT + 2 + (NEXT_BLOC_WEIGHT / 2 - Block_Max_Lenth(game->id_next_block));
    int y = NEXT_BLOC_HEIGHT - 3;

    // On vide l'emplacement
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < NEXT_BLOC_WEIGHT - 2; j++) {
            mvaddwstr(y + i, x, L" ");
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
            Debug("=-=-=-= token %ls =-=-=-=\n", token);
            attron(COLOR_PAIR(game->next_block.color));
            mvaddwstr(y + i, x, token); // Pour le mettre au milieu
            attroff(COLOR_PAIR(game->next_block.color));
            refresh();
            token = wcstok(NULL, L"\n", &saved);
            i++;
        }

        Debug("next pos (x : %d - y : %d)", x, y);
    }
}

int Put_Block(APIGame *game) {
    // On vérifie si il y a un block en dessous
    /*if (Block_Physics(game)) {
        return 1;
    }*/

    // On supprimer l'ancienne position du bloc
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    int i = 0;
    wchar_t *saved1, *saved2;
    wchar_t *token = wcstok(buffer, L"\n", &saved1);
    while (token != NULL) {
        Debug("Suppression de la ligne (%d) - %ls", game->pos.y + i, game->lines[game->pos.y + i]);
        wmemset(game->lines[game->pos.y + i] + game->pos.x, L' ', wcslen(token));

        wchar_t clear_token[wcslen(token) + 1];
        wmemset(clear_token, L' ', wcslen(token));
        clear_token[wcslen(token)] = L'\0';
        Debug("token '%ls'\n", token);
        Debug("clear token = '%ls'\n", clear_token);
        Debug("pos (x : %d - y : %d)\n", game->pos.x, game->pos.y + i);
        mvaddwstr(game->pos.y + i, game->pos.x, clear_token);
        refresh();

        token = wcstok(NULL, L"\n", &saved1);
        i++;
    }

    // On descend le bloc
    game->pos.y++;
    //napms(2000);

    // Ajoute le bloc à la nouvelle position
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    i = 0;
    token = wcstok(buffer, L"\n", &saved2);
    while (token != NULL) {
        Debug("Ajout de %ls à la ligne (%d) - %ls", token, game->pos.y + i, game->lines[game->pos.y + i]);
        wmemcpy(game->lines[game->pos.y + i] + game->pos.x, token, wcslen(token));

        // On ajoute le bout de block sur l"écran
        attron(COLOR_PAIR(game->block.color));
        mvaddwstr(game->pos.y + i, game->pos.x, token);
        refresh();
        attroff(COLOR_PAIR(game->block.color));

        token = wcstok(NULL, L"\n", &saved2);
        i++;
    }

    // Pour le debug
    Debug("pos y = %d\n", game->pos.y);
    Debug("next\n%ls\n", game->next_block.shape);
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        Debug("%ls", game->lines[i]);
    }
    printf("\n");
    return 0;
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

    int random_bloc;
    if (id_block == -1) {
        random_bloc = rand() % (sizeof(block) / sizeof(block[0]));
    } else {
        random_bloc = id_block;
    }
    int random_x = ((rand() % (GAME_WEIGHT - Block_Max_Lenth(random_bloc))) / 2) * 2 + 2;

    game->pos.x = random_x;
    game->pos.y = 1;

    if (game->block.shape == NULL) {
        game->block.shape = wcsdup(block[random_bloc].shape);
        game->block.color = block[random_bloc].color;
        game->id_block = random_bloc;

        random_bloc = rand() % (sizeof(block) / sizeof(block[0]));
        game->next_block.shape = wcsdup(block[random_bloc].shape);
        game->next_block.color = block[random_bloc].color;
        game->id_next_block = random_bloc;
    } else {
        game->block.shape = game->next_block.shape;
        game->block.color = game->next_block.color;
        game->id_block = game->id_next_block;

        game->next_block.shape = block[random_bloc].shape;
        game->next_block.color = block[random_bloc].color;
        game->id_next_block = random_bloc;
    }

    // On fait une copie parce que strtok est chiant
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, block[random_bloc].shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = '\0';

    int i = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token != NULL) {
        // On stock le bloc à la bonne position
        wmemcpy(game->lines[game->pos.y + i] + game->pos.x, token, wcslen(token));
        
        // On ajoute le bout de block sur l"écran
        attron(COLOR_PAIR(game->block.color));
        mvaddwstr(game->pos.y + i, game->pos.x, token); // On fait + 1 car game->lines le premier indice c'est 0, or dans l'affichage 0 c'est la bordure donc on a un décalage de 1
        attroff(COLOR_PAIR(game->block.color));
        
        token = wcstok(NULL, L"\n", &saved);
        i++;
    }

    // Pour le debug
    Debug("pos y = %d\n", game->pos.y);
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
        return -1;
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
        return -1;
    }

    // On initialise toutes les lignes avec des ' '
    for (int i = 0; i <= GAME_HEIGHT + 1; i++) {
        game.lines[i] = malloc((GAME_WEIGHT + 4) * sizeof(wchar_t));
        if (!game.lines[i]) {
            Error("N'as pas réussis à allouer de la mémoire.");
            return -1;
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
            Debug("On spawn (ligne %d)\n%ls\n", game.pos.y, game.block.shape);
            has_spawned = 1;
        } else if (Put_Block(&game)) {
            game.pos.y = 0;
            has_spawned = 0;
        }
	    refresh();

        Put_Next_Block(&game);

        // On attends un peu avant la prochaine position du bloc
	    napms(BLOCK_WAIT);

        // On vérifie si il est en bas
	    size_t height = Block_Size(game.block.shape) == 1 ? GAME_HEIGHT : GAME_HEIGHT - 1;
	    Debug("pos %d - height %ld\n", game.pos.y, height);
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
    return 0;
}
