#include "../headers/ihm.h"

int Clear_Full_Lines(APIGame *game) {
    // On vérifie si une ligne est pleine
    int full_cases = 0;
    int full_count = 0;
    int *full_lines = NULL;

    Debug("pos y : %d - max : %d\n", game->pos.y, game->pos.y + Get_End_Of_Block(game));
    for (int y = game->pos.y; y < GAME_API_HEIGHT - 1 && y <= game->pos.y + Get_End_Of_Block(game); y++) {
        for (int x = 1; x < GAME_API_WEIGHT - 1; x++) {
            // Si un bloc est présent un incrémente notre compteur
            Debug("On test : x : %d - y : %d\n", x, y);
            if (game->grid[y][x]) {
                Debug("Oui %d\n", game->grid[y][x]);
                full_cases++;
            }
        }
        if (full_cases == GAME_API_WEIGHT - 2) {
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
    if (!full_count) return SUCCESS;

    // On supprime toutes les lignes full
    for (int i = 0; i < full_count; i++) {
        for (int x = 1; x < GAME_API_WEIGHT - 1; x++) {
            Debug("test de %d\n", full_lines[i]);
            Debug("Donc %d\n", game->grid[full_lines[i]][x]);
            game->grid[full_lines[i]][x] = 0;
        }
    }

    Debug("C'est fait.\n");

    // On récupère l'index de la première ligne vide (en partant du bas)
    int max_bloc_y = 0;
    for (int y = full_lines[0] - 1; y > 0; y--) {
        for (int x = 1; x < GAME_API_WEIGHT - 1; x++) {
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
            for (int x = 1; x < GAME_API_WEIGHT - 1; x++) {
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

int **Clone_Block(int **src, int size) {
    int **copy = malloc(size * sizeof(int *));
    if (!copy) return NULL;

    for (int i = 0; i < size; i++) {
        copy[i] = malloc(size * sizeof(int));
        if (!copy[i]) {
            // Nettoie en cas d'échec partiel
            for (int j = 0; j < i; j++) free(copy[j]);
            free(copy);
            return NULL;
        }
        memcpy(copy[i], src[i], size * sizeof(int));
    }

    return copy;
}

void Free_Block(int **block, int size) {
    for (int i = 0; i < size; i++) {
        free(block[i]);
    }
    free(block);
}

int Put_Next_Block(APIGame *game) {
    // On veux mettre le next_block pas le bloc, donc on save et on met le next_block à la place du bloc
    int id_block = game->id_block;
    int saved_id = game->id_block;
    int saved_size = Get_Block_Size(saved_id);
    int **saved_block = Clone_Block(game->block, saved_size);
    if (!saved_block) return ERROR;

    // On replace temporairement par le bloc suivant
    int size = Get_Block_Size(game->id_block);
    game->id_block = game->id_next_block;
    if (Set_Block(game, IS_BLOCK, size)) return ERROR;

    // On calcul la positon pour le bloc
    int x = GAME_API_WEIGHT - 1 + (NEXT_BLOCK_WEIGHT - 2) / GAME_WEIGHT_MUL / 2 + 1 - (Get_Block_Width(game) + 1) / 2; // Ouais elle a le cancer je sais
    int y = NEXT_BLOCK_HEIGHT - 3;

    Debug("On place la prochaie pièce (%d) à x : %d - y : %d\n", id_block, x, y);
    Debug("Détails : %d + (%d - 2) / %d / 2 + 1 - (%d - 1) / 2\n", GAME_API_WEIGHT, NEXT_BLOCK_WEIGHT, GAME_WEIGHT_MUL, Get_Block_Width(game));
    Display_Block("", game);

    Del_Next_Block(game, GAME_HEIGHT + 1, 3);
    Put_Block(game, x, y);
    
    Display_Block("lalala :", game);
    // On ne veux pas vraiment passer au bloc suivant, on veux juste l'affiché donc en remet l'ancien bloc
    Free_Block(game->block, saved_size);
    game->block = saved_block;
    game->id_block = saved_id;
    
    Display_Block("On a après :", game);
    return SUCCESS;
}

void Del_Next_Block(APIGame *game, const int posX, const int posY) {
    // On cherche tout les blocs non vides
    for (int y = 0; y < 2; y++) {
        ColorList color_list = Get_Color_List();

        Debug("On met à jour l'affichage avec '%ls' au coordonnées x : %d - y : %d\n", CLEAR_NEXT_BLOCK, posX, y);
        // On place le bloc sur l'écran
        attron(COLOR_PAIR(color_list.colors[game->id_block]));
        mvaddwstr(y + posY, posX, CLEAR_NEXT_BLOCK);
        attroff(COLOR_PAIR(color_list.colors[game->id_block]));
        refresh();
    }
    DebugSimple("\n");
}

void Update_Block(APIGame *game, const int posX, const int posY, const wchar_t *state) {
    int back_posX = posX;
    int back_posY = posY;
    int size = Get_Block_Size(game->id_block);

    Debug("On veux placer '%ls' à x : %d - y : %d\n", state, posX, posY);
    Display_Block("La pièce est :", game);

    // On cherche tout les blocs non vides
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // Si on a un block
            if (game->block[y][x]) {
                // On met à jours la grille
                if (posX < GAME_API_WEIGHT && posY < GAME_API_HEIGHT) {
                    Debug("On met à jour l'API avec game->grid[%d][%d] = %d\n", back_posY, back_posX, wcscmp(state, BLOCK) == 0 ? game->block[y][x] : 0);
                    game->grid[back_posY][back_posX] = wcscmp(state, BLOCK) == 0 ? game->block[y][x] : 0;
                }

                ColorList color_list = Get_Color_List();

                Debug("On met à jour l'affichage avec '%ls' au coordonnées x : %d - y : %d\n", state, back_posX, back_posY);
                // On place le bloc sur l'écran
                attron(COLOR_PAIR(color_list.colors[game->id_block]));
                mvaddwstr(back_posY, (back_posX - 1) * GAME_WEIGHT_MUL + 1, state);
                attroff(COLOR_PAIR(color_list.colors[game->id_block]));
                refresh();
            }
            back_posX++;
        }
        back_posX = posX;
        back_posY++;
    }
    DebugSimple("\n");
}

int Place_Block(APIGame *game) {
    Debug("On supprime l'ancienne position du bloc.\n");
    // On supprimer l'ancienne position du bloc
    Del_Block(game, game->pos.x, game->pos.y);
    Display_Grid("On a en nouvelle grille :", game);

    int save_x = game->pos.x;
    int save_y = game->pos.y;

    // On descend le bloc
    if (game->direction == GO_LEFT) {
        game->pos.x--;
    } else if (game->direction == GO_RIGHT) {
        game->pos.x++;
    } else if (game->direction == GO_DOWN) {
        game->pos.y++;
    } else if (game->direction == GO_UP) {
        Rotate_Block(game);
    }

    // On vérifie si il y a un block en dessous
    Debug("On test à x : %d - y : %d\n", game->pos.x, game->pos.y);
    int colision = 0;
    if (game->pos.x < GAME_API_WEIGHT - 1 && game->pos.y < GAME_API_HEIGHT - 1) {
        Debug("On vérifie si il y a des colisions.\n");
        int ret = Block_Physics(game);
        Debug("ret ???? : %d\n", ret);
        if (ret == LOOSE) {
            return ret;
        } else if (ret) {
            game->pos.x = save_x;
            game->pos.y = save_y;
            colision = ret;
            if (game->direction == GO_UP) Cancel_Rotate(game);
        }
    }
    Debug("On a au final à x : %d - y : %d\n", game->pos.x, game->pos.y);

    Debug("On ajoute le bloc à la nouvelle position.\n");
    // Ajoute le bloc à la nouvelle position
    Put_Block(game, game->pos.x, game->pos.y);
    Display_Grid("On a donc cette nouvelle grille :", game);

    // On met à jour la grille si il y a des lignes full
    if (colision) {
        int ret = Clear_Full_Lines(game);
        if (ret == CLEAR_BLOCK) {
            Refresh_Grid(game);
        } else if (ret) {
            return ret;
        }
        Display_Grid("On a alors :", game);
    }
    return colision;
}

void Borders(const wchar_t *c1, const wchar_t *c2) {
    // Pour crée la bordure haut ou bas
    addwstr(c1);
    for(int i = 0; i < GAME_WEIGHT - 2; i++) {
        addwstr(MIDLESCORE);
    }
    addwstr(c2);
}

void Refresh_Game(APIGame *game) {
    Refresh_Grid(game);
    // On refresh aussi le next bloc
    Put_Next_Block(game);
}

void Refresh_Grid(APIGame *game) {
    for (int y = 1; y < GAME_API_HEIGHT - 1; y++) {
        for (int x = 1; x < GAME_API_WEIGHT - 1; x++) {
            ColorList color_list = Get_Color_List();
            attron(COLOR_PAIR(color_list.colors[game->grid[y][x]]));
            game->grid[y][x] ? mvaddwstr(y, (x - 1) * GAME_WEIGHT_MUL + 1, L"[]") : mvaddwstr(y, (x - 1) * GAME_WEIGHT_MUL + 1, L"  ");
            attron(COLOR_PAIR(color_list.colors[game->grid[y][x]]));
        }
    }
}

void Create_Frame_Next_Block() {
    mvaddwstr(0, GAME_WEIGHT, CORNER_TOP_LEFT);
    for (int i = GAME_WEIGHT + 1; i < GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1; i++) {
        mvaddwstr(0, i, MIDLESCORE);
    }
    mvaddwstr(0, GAME_WEIGHT + NEXT_BLOCK_WEIGHT  - 1, CORNER_TOP_RIGHT);

    move(1, GAME_WEIGHT);
    addwstr(WALL);
    addwstr(L" Next block ");
    addwstr(WALL);

    mvaddwstr(2, GAME_WEIGHT, CROSS_LEFT);
    for (int i = GAME_WEIGHT + 1; i < GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1; i++) {
        mvaddwstr(2, i, MIDLESCORE);
    }
    mvaddwstr(2, GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1, CROSS_RIGHT);

    for (int i = 0; i < 2; i++) {
        mvaddwstr(3 + i, GAME_WEIGHT, WALL);
        for (int j = GAME_WEIGHT + 1; j < GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1; j++) {
            mvaddwstr(3 + i, j, L" ");
        }
        mvaddwstr(3 + i, GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1, WALL);
    }

    mvaddwstr(NEXT_BLOCK_HEIGHT - 1, GAME_WEIGHT, CORNER_BOT_LEFT);
    for (int i = GAME_WEIGHT + 1; i < GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1; i++) {
        mvaddwstr(NEXT_BLOCK_HEIGHT - 1, i, MIDLESCORE);
    }
    mvaddwstr(NEXT_BLOCK_HEIGHT - 1, GAME_WEIGHT + NEXT_BLOCK_WEIGHT - 1, CORNER_BOT_RIGHT);
}

void Create_Info() {
    mvaddwstr(GAME_HEIGHT - 9, GAME_WEIGHT + 1, L"  ↑ : Rotate");
    mvaddwstr(GAME_HEIGHT - 8, GAME_WEIGHT + 1, L"  ← : Right");
    mvaddwstr(GAME_HEIGHT - 7, GAME_WEIGHT + 1, L"  → : Left");
    mvaddwstr(GAME_HEIGHT - 6, GAME_WEIGHT + 1, L"  ↓ : Down");


    mvaddwstr(GAME_HEIGHT - 4, GAME_WEIGHT + 1, L"  +/- : Speed Up/Down");
    mvaddwstr(GAME_HEIGHT - 3, GAME_WEIGHT + 1, L"  p   : Pause");
    mvaddwstr(GAME_HEIGHT - 2, GAME_WEIGHT + 1, L"  q   : Quit");
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
    for (int i = 0; i < GAME_HEIGHT - 2; i++) {
	    addwstr(WALL);
	    for (int j = 0; j < GAME_WEIGHT - 2; j++) {
	        addwstr(L" ");
	    }
	    addwstr(WALL);
	    addwstr(L"\n");
    }

    Bottom_Border();
    addwstr(L"\n");

    Create_Frame_Next_Block();
    Create_Info();
}

/* Gestion de la game */
int Get_New_Block(APIGame *game) {
    Spawn(game);

    // On vérifie si on peut placer le bloc avant de le faire spawn
    if (Block_Physics(game)) {
        return LOOSE;
    }

    Put_Block(game, game->pos.x, game->pos.y);
    return SUCCESS;
}

int Game(APIGame *game) {
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

    clear();
    Create_Frame(); // On crée le cadre
    if (game->flag == FLAG_DEBUG) {
        has_spawned = 1;
        game->flag = FLAG_NEUTRAL;
        Refresh_Game(game);
        clock_gettime(CLOCK_MONOTONIC, &start);
        Debug("Jeu chargé.\n");
    }

    while(1) {
        if (!paused) {
            // On vérifie si un bloc à déjà spawn pour pas en faire apparaire plusieurs
            if (!has_spawned) {
                Debug("On fait spawn un nouveau bloc.\n");
                int ret = Get_New_Block(game);
                Display_Block("On a maintenant un nouveau bloc :", game);
                Display_Next_Block("Et le prochaine bloc est :", game);

                if (ret) {
                    endwin();
                    return ret;
                }
                if (Put_Next_Block(game)) return ERROR;
                has_spawned = 1;
                clock_gettime(CLOCK_MONOTONIC, &start);
            } else {
                clock_gettime(CLOCK_MONOTONIC, &current);
                long elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_nsec - start.tv_nsec) / 1000000;

                if (elapsed >= BLOCK_WAIT || !has_spawned) {
                    start = current;
                    Display_Block("On va placer le bloc actuelle :", game);
                    game->direction = GO_DOWN;
                    int ret = Place_Block(game);
                    if (ret) {
                        if (ret == LOOSE) {
                            endwin();
                            return ret;
                        } else if (ret == COLISION) {
                            game->pos.y = 0;
                            has_spawned = 0;
                        }
                    }
                }
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
                        Display_Grid("Status grille :", game);
                        Display_Block("Status block :", game);
                        Display_Next_Block("Status next block :", game);
                    }
                    paused = !paused;
                    // Mettre un menu de pause
                    break;
                case 'q':
                    Debug("Exit : %d\n", EXIT);
                    endwin();
                    return EXIT;
                    break;
                case KEY_DOWN:
                    Debug("DOWN\n");
                    int true_y = Get_End_Of_Block(game);
                    if (true_y < GAME_HEIGHT && game->pos.y > 1 && !paused) {
                        game->direction = GO_DOWN;
                        int ret = Place_Block(game);
                        if (ret) {
                            if (ret == LOOSE) {
                                endwin();
                                return ret;
                            } else if (ret == COLISION) {
                                game->pos.y = 0;
                                has_spawned = 0;
                            }
                        }
                    }
                    break;
                case KEY_LEFT:
                    Debug("LEFT\n");
                    if (game->pos.y > 1 && !paused) {
                        game->direction = GO_LEFT;
                        int ret = Place_Block(game);
                        if (ret) {
                            if (ret == LOOSE) {
                                endwin();
                                return ret;
                            } else if (ret == COLISION) {
                                game->pos.y = 0;
                                has_spawned = 0;
                            }
                        }
                    }
                    break;
                case KEY_RIGHT:
                    Debug("RIGHT\n");
                    if (game->pos.x < GAME_API_WEIGHT - Get_Block_Width(game) && game->pos.y > 1 && !paused) {
                        game->direction = GO_RIGHT;
                        int ret = Place_Block(game);
                        if (ret) {
                            if (ret == LOOSE) {
                                endwin();
                                return ret;
                            } else if (ret == COLISION) {
                               game->pos.y = 0;
                                has_spawned = 0;
                            }
                        }
                    }
                    break;
                case KEY_UP:
                    Debug("UP\n");
                    Debug("huu : %d - %d ; x : %d\n", GAME_API_WEIGHT, Get_Block_Size(game->id_block), game->pos.x);
                    if (game->pos.x > 0 && game->pos.x + Get_Block_Size(game->id_block) < GAME_API_WEIGHT && !paused) {
                        // On vérifie si on peut tourner dans Place_Block
                        game->direction = GO_UP;
                        int ret = Place_Block(game);
                        if (ret) {
                            if (ret == LOOSE) {
                                endwin();
                                return ret;
                            } else if (ret == COLISION) {
                            game->pos.y = 0;
                                has_spawned = 0;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    endwin();
    return SUCCESS;
}