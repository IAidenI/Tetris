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

int Put_Block(APIGame *game) {
    // On vérifie si il y a un block en dessous
    if (Block_Physics(game)) {
        return 1;
    }

    // On supprimer l'ancienne position du bloc
    wchar_t buffer[MAX_COPY];
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    int i = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token != NULL) {
        Debug("Suppression de la ligne (%d) - %ls", game->pos.y + i, game->lines[game->pos.y + i]);
        wmemset(game->lines[game->pos.y + i] + game->pos.x, L' ', wcslen(token));

        wchar_t clear_token[wcslen(token) + 1];
        wmemset(clear_token, L' ', wcslen(token));
        clear_token[wcslen(token)] = L'\0';
        mvaddwstr(game->pos.y + i, game->pos.x, L" ");
        refresh();

        token = wcstok(NULL, L"\n", &saved);
        i++;
    }

    // On descend le bloc
    game->pos.y++;

    // Ajoute le bloc à la nouvelle position
    wmemset(buffer, L'\0', MAX_COPY);
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);
    buffer[MAX_COPY - 1] = L'\0';

    i = 0;
    token = wcstok(buffer, L"\n", &saved);
    while (token != NULL) {
        Debug("Ajout de %ls à la ligne (%d) - %ls", token, game->pos.y + i, game->lines[game->pos.y + i]);
        wmemcpy(game->lines[game->pos.y + i] + game->pos.x, token, wcslen(token));

        // On ajoute le bout de block sur l"écran
        attron(COLOR_PAIR(game->block.color));
        mvaddwstr(game->pos.y + i, game->pos.x, token);
        refresh();
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
    return 0;
}

void Borders(const wchar_t *c1, const wchar_t *c2) {
    addwstr(c1);
    for(int i = 0; i < GAME_WEIGHT; i++) {
        addwstr(MIDLESCORE);
    }
    addwstr(c2);
    addwstr(L"\n");
}

void Create_Frame() {
    clear();
    Top_Border();

    for (int i = 0; i < GAME_HEIGHT; i++) {
	    addwstr(WALL);
	    for (int j = 0; j < GAME_WEIGHT; j++) {
	        addwstr(L" ");
	    }
	    addwstr(WALL);
	    addwstr(L"\n");
    }

    Bottom_Border();
}

/* Gestion de la game */
void Spawn(APIGame *game) {
    srand(time(NULL));

    int random_bloc = rand() % (sizeof(block) / sizeof(block[0]));
    int random_x = ((rand() % (GAME_WEIGHT - Block_Max_Lenth(random_bloc))) / 2) * 2 + 2;

    game->pos.x = random_x;
    game->pos.y = 1;
    game->block.shape = wcsdup(block[random_bloc].shape);
    game->block.color = block[random_bloc].color;

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

int Game() {
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
            Spawn(&game);
            Debug("On spawn (ligne %d)\n%ls\n", game.pos.y, game.block.shape);
            has_spawned = 1;
        } else if (Put_Block(&game)) {
            game.pos.y = 0;
            has_spawned = 0;
        }
	    refresh();

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
