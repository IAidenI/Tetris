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
    wcsncpy(buffer, shape, MAX_COPY - 1);

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
    wcsncpy(buffer, block[i].shape, MAX_COPY - 1);


    size_t lenth = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token) {
        lenth = wcslen(token) > lenth ? wcslen(token) : lenth;
        token = wcstok(NULL, L"\n", &saved);
    }
    return lenth;
}

void Refresh_Frame(APIGame *game) {
    // strtok ceux pas de const donc on fait une copie de la forme
    wchar_t buffer[MAX_COPY];
    wcsncpy(buffer, game->block.shape, MAX_COPY - 1);

    // On affiche le bloc
    clear();
    Top_Border();

    size_t size = Block_Size(game->block.shape);
    for (int i = 0; i < GAME_HEIGHT; i++) {
        if (i == game->pos.y || size != 0) {
            attron(COLOR_PAIR(game->block.color));
            addwstr(game->lines[i]);
            attroff(COLOR_PAIR(game->block.color));
            size--;
        } else {
            addwstr(game->lines[i]);
        }
    }

    Bottom_Border();
    
    // Pour le debug
    Debug("######################\n");
    for (int i = 0; i < GAME_HEIGHT; i++) {
        Debug("%ls", game->lines[i]);
    }
    Debug("######################\n\n");
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
    game->pos.y = 0;
    game->block.shape = wcsdup(block[random_bloc].shape);
    game->block.color = block[random_bloc].color;

    // On fait une copie parce que strtok est chiant
    wchar_t buffer[MAX_COPY];
    wcsncpy(buffer, block[random_bloc].shape, MAX_COPY - 1);

    int j = 0;
    wchar_t *saved;
    wchar_t *token = wcstok(buffer, L"\n", &saved);
    while (token != NULL) {
        size_t len = wcslen(token);
        wchar_t *formated = malloc((GAME_WEIGHT + 4) * sizeof(wchar_t));
        if (!formated) {
            Error("N'as pas réussis à allouer de la mémoire.");
            exit(1);
        }

        // Rempli d'espaces avant le bloc
        wmemset(formated, L' ', GAME_WEIGHT + 3);

        // On place les bords
        formated[0] = *WALL;
        formated[GAME_WEIGHT + 1] = *WALL;
        formated[GAME_WEIGHT + 2] = L'\n';
        formated[GAME_WEIGHT + 3] = L'\0';

        // Copie le bloc à la bonne position
        wmemcpy(formated + game->pos.x, token, len);

        // On le stock
        game->lines[game->pos.y + j] = formated;
        token = wcstok(NULL, L"\n", &saved);
        j++;
    }
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

    // Création de l'API game pour pouvoir gerer correctement la victoire ou autre
    APIGame game;
    game.pos.x = 0;
    game.pos.y = 0;
    game.block.shape = wcsdup(block[0].shape);
    game.block.color = block[0].color;
    if (!game.block.shape) {
        Error("N'as pas réussis à allouer de la mémoire.");
        return -1;
    }
    game.lines = malloc(GAME_HEIGHT * sizeof(wchar_t *));
    if (!game.lines) {
        Error("N'as pas réussis à allouer de la mémoire.");
        return -1;
    }

    // On initialise toutes les lignes avec des ' '
    for (int i = 0; i < GAME_HEIGHT; i++) {
        game.lines[i] = malloc((GAME_WEIGHT + 4) * sizeof(wchar_t));
        if (!game.lines[i]) {
            Error("N'as pas réussis à allouer de la mémoire.");
            return -1;
        }
        wmemset(game.lines[i] + 1, L' ', GAME_WEIGHT + 3);

        // On ajoute les bords
        game.lines[i][0] = *WALL;
        game.lines[i][GAME_WEIGHT + 1] = *WALL;
        game.lines[i][GAME_WEIGHT + 2] = L'\n';
        game.lines[i][GAME_WEIGHT + 3] = L'\0';
    }

    while (1) {
        clear();

        // On crée le cadre
        //Create_Frame();

        // Si on a atteint le bas ou si c'est le début on fait spawn un bloc
        size_t height = Block_Size(game.block.shape) == 1 ? GAME_HEIGHT : GAME_HEIGHT - 1;
        if (game.pos.y == height || game.pos.y == 0) { Spawn(&game); }
        Refresh_Frame(&game);
	    refresh();

        // On attends un peu avant la prochaine position du bloc
	    napms(BLOCK_WAIT);
	    game.pos.y++;

        // On vérifie si il est en bas
	    height = Block_Size(game.block.shape) == 1 ? GAME_HEIGHT : GAME_HEIGHT - 1;
	    if (game.pos.y > height) {
	        key = getch(); // On récupère l'ebtré utilisateur
	        if (temp == 3) {
	            break;
	        }
	        temp++;
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
