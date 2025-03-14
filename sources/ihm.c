#include "../headers/ihm.h"

void Put_Next_Block(APIGame *game) {
    // On calcul la positon pour le bloc
    int x = (int)(GAME_WEIGHT + 2 + ((NEXT_BLOCK_WEIGHT + 2) / 2 - Get_Block_Width(game)));
    int y = NEXT_BLOCK_HEIGHT - 1;

    // On veux mettre le next_block pas le bloc, donc on save et on met le next_block à la place du bloc
    int id_block = game->id_block;
    game->block = game->next_block;
    game->id_block = game->id_next_block;
    Debug("On place la prochaime pièce.\n");
    Del_Block(game, x, y);
    Put_Block(game, x, y);
    // On ne veux pas vraiment passer au bloc suivant, on veux juste l'affiché donc en remet l'ancien bloc
    int (**shapes)[BLOCK_SIZE] = Get_Shapes();
    game->block = shapes[0];
    game->block = shapes[id_block];
    game->id_block = id_block;
    fprintf(stderr, "\n");
}

void Update_Block(APIGame *game, const int posX, const int posY, const wchar_t *state) {
    int back_posX = posX;
    int back_posY = posY;

    Debug("On veux placer '%ls' à x : %d - y : %d\n", state, posX, posY);
    // On cherche tout les blocs non vides
    for (int y = 0; y < BLOCK_SIZE; y++) {
        for (int x = 0; x < BLOCK_SIZE; x++) {
            // Si on a un block1
            if (game->block[y][x]) {
                // On met à jours la grille
                if (posX < GAME_API_WEIGHT && posY < GAME_API_HEIGHT) {
                    Debug("On met à jour l'API avec game->grid[%d][%d] = %d\n", back_posY, back_posX / GAME_WEIGHT_MUL, wcscmp(state, BLOCK) == 0 ? game->block[y][x] : 0);
                    game->grid[back_posY][back_posX / GAME_WEIGHT_MUL] = wcscmp(state, BLOCK) == 0 ? game->block[y][x] : 0;
                }

                ColorList color_list = Get_Color_List();

                Debug("On met à jour l'affichage avec '%ls' au coordonnées x : %d - y : %d\n", state, back_posX, back_posY);
                // On place le bloc sur l'écran
                attron(COLOR_PAIR(color_list.colors[game->id_block]));
                mvaddwstr(back_posY, back_posX, state);
                attroff(COLOR_PAIR(color_list.colors[game->id_block]));
                refresh();
            }
            back_posX++;
            back_posX++; // Car on est sur deux caractères pour une case
        }
        back_posX = posX;
        back_posY++;
    }
    fprintf(stderr, "\n");
}

int Place_Block(APIGame *game, const int direction) {
    Debug("On supprime l'ancienne position du bloc.\n");
    // On supprimer l'ancienne position du bloc
    Del_Block(game, game->pos.x, game->pos.y);

    int save_x = game->pos.x;
    int save_y = game->pos.y;

    // On descend le bloc
    if (direction == GO_LEFT) {
        game->pos.x--;
        game->pos.x--; // Parce que une case fait deux caractères
    } else if (direction == GO_RIGHT) {
        game->pos.x++;
        game->pos.x++; // Parce que une case fait deux caractères
    } else if (direction == GO_DOWN) {
        game->pos.y++;
    }

    // On vérifie si il y a un block en dessous
    Debug("On test à x : %d - y : %d\n", game->pos.x, game->pos.y);
    int colision = 0;
    if (game->pos.x <= GAME_API_WEIGHT && game->pos.y <= GAME_API_HEIGHT) {
        Debug("On vérifie si il y a des colisions.\n");
        int ret = Block_Physics(game);
        if (ret == LOOSE) {
            return ret;
        } else if (ret == ERROR) {
            Debug("Colision détecté.\n");
            game->pos.x = save_x;
            game->pos.y = save_y;
            colision = ret;
        }
    }
    Debug("On a au final à x : %d - y : %d\n", game->pos.x, game->pos.y);

    Debug("On ajoute le bloc à la nouvelle position.\n");
    // Ajoute le bloc à la nouvelle position
    Put_Block(game, game->pos.x, game->pos.y);
    Debug("On donc cette nouvelle grille :\n");
    for (int i = 0; i <= GAME_API_HEIGHT + 1; i++) {
        Debug();
        for (int j = 0; j <= GAME_API_WEIGHT; j++) {
            fprintf(stderr, "%d ", game->grid[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
    return colision;
}

void Borders(const wchar_t *c1, const wchar_t *c2) {
    // Pour crée la bordure haut ou bas
    addwstr(c1);
    for(int i = 0; i < GAME_WEIGHT; i++) {
        addwstr(MIDLESCORE);
    }
    addwstr(c2);
}

void Create_Frame() {
    // On crée la top bordure pour la game
    clear();
    Top_Border();

    // On ajoute la top bordure pour la partie Next Block
    addwstr(CORNER_TOP_LEFT);
    for (int j = 0; j < NEXT_BLOCK_IHM_LEN; j++) {
        addwstr(MIDLESCORE);
    }
    addwstr(CORNER_TOP_RIGHT);
    addwstr(L"\n");

    // On crée la grille
    for (int i = 0; i < GAME_HEIGHT; i++) {
	    addwstr(WALL);
	    for (int j = 0; j < GAME_WEIGHT; j++) {
	        addwstr(L" ");
	    }
	    addwstr(WALL);

        // Pour ajouter la Next Block
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
int Get_New_Block(APIGame *game) {
    Spawn(game);

    // On vérifie si on peut placer le bloc avant de le faire spawn
    if (Is_Block_Under(game)) {
        return LOOSE;
    }

    Put_Block(game, game->pos.x, game->pos.y);
    return SUCCESS;
}

int Game() {
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
    timeout(BLOCK_WAIT);
    use_default_colors();
    Init_Colors();

    int key;
    int has_spawned = 0;
    int paused = 0;
    struct timespec current, start;
    
    APIGame game;
    if (Start_Game(&game)) {
        return ERROR;
    }
    Debug("APIGame initialisé.\n");
    Debug("On a notre grille :\n");
    for (int i = 0; i <= GAME_API_HEIGHT + 1; i++) {
        Debug();
        for (int j = 0; j <= GAME_API_WEIGHT; j++) {
            fprintf(stderr, "%d ", game.grid[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");

    clear();
    Create_Frame(); // On crée le cadre
    while(1) {
        if (!paused) {
            // On vérifie si un bloc à déjà spawn pour pas en faire apparaire plusieurs
            if (!has_spawned) {
                Debug("On fait spawn un nouveau bloc.\n");
                int ret = Get_New_Block(&game);
                Debug("On a maintenant un nouveau bloc :\n");
                for (int i = 0; i < BLOCK_SIZE; i++) {
                    Debug();
                    for (int j = 0; j < BLOCK_SIZE; j++) {
                        fprintf(stderr, "%d ", game.block[i][j]);
                    }
                    fprintf(stderr, "\n");
                }
                Debug("Et le prochaine bloc est :\n");
                for (int i = 0; i < BLOCK_SIZE; i++) {
                    Debug();
                    for (int j = 0; j < BLOCK_SIZE; j++) {
                        fprintf(stderr, "%d ", game.next_block[i][j]);
                    }
                    fprintf(stderr, "\n");
                }
                fprintf(stderr, "\n");

                if (ret) {
                    return ret;
                }
                Put_Next_Block(&game);
                has_spawned = 1;
                clock_gettime(CLOCK_MONOTONIC, &start);
            } else {
                clock_gettime(CLOCK_MONOTONIC, &current);
                long elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_nsec - start.tv_nsec) / 1000000;

                if (elapsed >= BLOCK_WAIT || !has_spawned) {
                    start = current;
                    Debug("On va placer le bloc actuelle :\n");
                    for (int i = 0; i < BLOCK_SIZE; i++) {
                        Debug();
                        for (int j = 0; j < BLOCK_SIZE; j++) {
                            fprintf(stderr, "%d ", game.block[i][j]);
                        }
                        fprintf(stderr, "\n");
                    }
                    fprintf(stderr, "\n");
                    int ret = Place_Block(&game, GO_DOWN);
                    if (ret) {
                        if (ret == LOOSE) {
                            return ret;
                        }
                        game.pos.y = 0;
                        has_spawned = 0;
                    }
                }
                Put_Next_Block(&game);
            }
        }

	    key = getch();
        if (key != ERR) {
            switch (key) {
                case 'p':
                    if (paused) {
                        Debug("Jeu relancé.\n");
                    } else {
                        Debug("Jeu mis en pause.\n");
                    }
                    paused = !paused;
                    // Mettre un menu de pause
                    break;
                case KEY_DOWN:
                    int true_y = Get_End_Of_Block(&game);
                    if (true_y < GAME_HEIGHT && game.pos.y > 1) {
                        Place_Block(&game, GO_DOWN);
                    }
                    break;
                case KEY_LEFT:
                    if (game.pos.x > 1 && game.pos.y > 1) {
                        Place_Block(&game, GO_LEFT);
                    }
                    break;
                case KEY_RIGHT:
                    if (game.pos.x < GAME_WEIGHT - Get_Block_Width(&game) * 2 && game.pos.y > 1) {
                        Place_Block(&game, GO_RIGHT);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    Stop_Game(&game);
    return SUCCESS;
}