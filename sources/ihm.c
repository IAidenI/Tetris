#include "../headers/ihm.h"

/*
    Game
*/
int Game(APIGame *game) {
    int key;
    int has_spawned = 0;
    int paused = 0;
    struct timespec current, start;
    timeout(game->state.speed);

    clear();
    Create_Frame(); // On crée le cadre
    if (game->flag == FLAG_DEBUG) {
        has_spawned = 1;
        game->flag = FLAG_NEUTRAL;
        Refresh_Game(game);
        clock_gettime(CLOCK_MONOTONIC, &start);
        Debug("Jeu chargé.\n");
    }

    Put_Score(game->state.score);
    Put_Level(game->state.level);

    while(1) {
        if (!paused) {
            // On vérifie si un bloc à déjà spawn pour pas en faire apparaire plusieurs
            if (!has_spawned) {
                Debug("On fait spawn un nouveau bloc.\n");
                Spawn(game);
                // On vérifie si on peut placer le bloc avant de le faire spawn
                if (Block_Physics(game)) {
                    endwin();
                    return LOOSE;
                }
                Debug("On place un block.\n");
                For_Each_Block(game, game->pos.x, game->pos.y, BLOCK, Update_Block_API);
                For_Each_Block(game, game->pos.x, game->pos.y, BLOCK, Update_Block_IHM);

                Display_Block("On a maintenant un nouveau bloc :", game);
                Display_Next_Block("Et le prochaine bloc est :", game);
                Display_Grid("Avec cette grille :", game);

                if (Put_Next_Block(game)) return ERROR;
                has_spawned = 1;
                clock_gettime(CLOCK_MONOTONIC, &start);
            } else {
                clock_gettime(CLOCK_MONOTONIC, &current);
                long elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_nsec - start.tv_nsec) / 1000000;

                if (elapsed >= game->state.speed || !has_spawned) {
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
                        paused = !paused;
                    } else {
                        paused = !paused;
                        Debug("Jeu mis en pause.\n");
                        Display_Grid("Status grille :", game);
                        Display_Block("Status block :", game);
                        Display_Next_Block("Status next block :", game);
                        
                        int check = 1;
                        while (check) {
                            if (Menu_Pause()) {
                                if (Menu_Confirm()) {
                                    return EXIT;
                                }
                                check = 1;
                            } else {
                                check = 0;
                            }
                        }
                        Refresh_Grid(game);
                        paused = !paused;
                    }
                    break;
                case 'q':
                    paused = !paused;
                    if (Menu_Confirm()) {
                        return EXIT;
                        Debug("Exit : %d\n", EXIT);
                        endwin();
                    }
                    Refresh_Grid(game);
                    paused = !paused;
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
                    if (game->pos.y > 1  && !paused) {
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
                    if (game->pos.x < GAME_API_WIDTH - Get_Block_Width(game) && game->pos.y > 1 && !paused) {
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
                    if (game->pos.x > 0 && game->pos.x + Get_Block_Size(game->id_block) < GAME_API_WIDTH && !paused) {
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



/*
    Relatif à la création du visuel
*/
void Create_Frame() {
    // On crée la top bordure pour la game
    clear();
    Top_Border();
    addwstr(L"\n");

    // On crée la grille
    for (int i = 0; i < GAME_HEIGHT - 2; i++) {
	    addwstr(WALL);
	    for (int j = 0; j < GAME_WIDTH - 2; j++) {
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

void Create_Frame_Next_Block() {
    mvaddwstr(0, GAME_WIDTH, CORNER_TOP_LEFT);
    for (int i = GAME_WIDTH + 1; i < GAME_WIDTH + NEXT_BLOCK_WIDTH - 1; i++) {
        mvaddwstr(0, i, MIDLESCORE);
    }
    mvaddwstr(0, GAME_WIDTH + NEXT_BLOCK_WIDTH  - 1, CORNER_TOP_RIGHT);

    move(1, GAME_WIDTH);
    addwstr(WALL);
    addwstr(L" Next block ");
    addwstr(WALL);

    mvaddwstr(2, GAME_WIDTH, CROSS_LEFT);
    for (int i = GAME_WIDTH + 1; i < GAME_WIDTH + NEXT_BLOCK_WIDTH - 1; i++) {
        mvaddwstr(2, i, MIDLESCORE);
    }
    mvaddwstr(2, GAME_WIDTH + NEXT_BLOCK_WIDTH - 1, CROSS_RIGHT);

    for (int i = 0; i < 2; i++) {
        mvaddwstr(3 + i, GAME_WIDTH, WALL);
        for (int j = GAME_WIDTH + 1; j < GAME_WIDTH + NEXT_BLOCK_WIDTH - 1; j++) {
            mvaddwstr(3 + i, j, L" ");
        }
        mvaddwstr(3 + i, GAME_WIDTH + NEXT_BLOCK_WIDTH - 1, WALL);
    }

    mvaddwstr(NEXT_BLOCK_HEIGHT - 1, GAME_WIDTH, CORNER_BOT_LEFT);
    for (int i = GAME_WIDTH + 1; i < GAME_WIDTH + NEXT_BLOCK_WIDTH - 1; i++) {
        mvaddwstr(NEXT_BLOCK_HEIGHT - 1, i, MIDLESCORE);
    }
    mvaddwstr(NEXT_BLOCK_HEIGHT - 1, GAME_WIDTH + NEXT_BLOCK_WIDTH - 1, CORNER_BOT_RIGHT);
}

void Create_Info() {
    mvaddwstr(GAME_HEIGHT - 9, GAME_WIDTH + 1, L"  ↑ : Rotate");
    mvaddwstr(GAME_HEIGHT - 8, GAME_WIDTH + 1, L"  ← : Right");
    mvaddwstr(GAME_HEIGHT - 7, GAME_WIDTH + 1, L"  → : Left");
    mvaddwstr(GAME_HEIGHT - 6, GAME_WIDTH + 1, L"  ↓ : Down");


    mvaddwstr(GAME_HEIGHT - 4, GAME_WIDTH + 1, L"  s : Snapshot");
    mvaddwstr(GAME_HEIGHT - 3, GAME_WIDTH + 1, L"  p : Pause");
    mvaddwstr(GAME_HEIGHT - 2, GAME_WIDTH + 1, L"  q : Quit");
}

void Borders(const wchar_t *c1, const wchar_t *c2) {
    // Pour crée la bordure haut ou bas
    addwstr(c1);
    for(int i = 0; i < GAME_WIDTH - 2; i++) {
        addwstr(MIDLESCORE);
    }
    addwstr(c2);
}



/*
    Relatif à l'affichage d'un bloc
*/
void Update_Block_IHM(APIGame *game, const int x, const int y, const int value, const wchar_t *state) {
    ColorList color_list = Get_Color_List();

    Debug("On met à jour l'affichage avec '%ls' au coordonnées x : %d - y : %d\n", state, x, y);
    // On place le bloc sur l'écran
    attron(COLOR_PAIR(color_list.colors[game->id_block]));
    mvaddwstr(y, (x - 1) * GAME_WIDTH_MUL + 1, state);
    attroff(COLOR_PAIR(color_list.colors[game->id_block]));
    refresh();
}

int Place_Block(APIGame *game) {
    Debug("On supprime l'ancienne position du bloc.\n");
    // On supprimer l'ancienne position du bloc
    //Del_Block(game, game->pos.x, game->pos.y);
    For_Each_Block(game, game->pos.x, game->pos.y, NO_BLOCK, Update_Block_API);
    For_Each_Block(game, game->pos.x, game->pos.y, NO_BLOCK, Update_Block_IHM);
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
    if (game->pos.x < GAME_API_WIDTH - 1 && game->pos.y < GAME_API_HEIGHT - 1) {
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
    //Put_Block(game, game->pos.x, game->pos.y);
    For_Each_Block(game, game->pos.x, game->pos.y, BLOCK, Update_Block_API);
    For_Each_Block(game, game->pos.x, game->pos.y, BLOCK, Update_Block_IHM);
    Display_Grid("On a donc cette nouvelle grille :", game);

    // On met à jour la grille si il y a des lignes full
    if (colision) {
        int ret = Clear_Full_Lines(game);
        if (ret == CLEAR_BLOCK) {
            Put_Score(game->state.score);
            Put_Level(game->state.level);
            Refresh_Grid(game);
        } else if (ret) {
            return ret;
        }
        Display_Grid("On a alors :", game);
    }
    return colision;
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
    int x = GAME_API_WIDTH - 1 + (NEXT_BLOCK_WIDTH - 2) / GAME_WIDTH_MUL / 2 + 1 - (Get_Block_Width(game) + 1) / 2; // Ouais elle a le cancer je sais
    int y = NEXT_BLOCK_HEIGHT - 3;

    Debug("On place la prochaie pièce (%d) à x : %d - y : %d\n", id_block, x, y);
    Debug("Détails : %d + (%d - 2) / %d / 2 + 1 - (%d - 1) / 2\n", GAME_API_WIDTH, NEXT_BLOCK_WIDTH, GAME_WIDTH_MUL, Get_Block_Width(game));
    Display_Block("", game);

    Del_Next_Block(game, GAME_HEIGHT + 1, 3);
    //Put_Block(game, x, y);
    For_Each_Block(game, x, y, BLOCK, Update_Block_IHM);
    
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



/*
    Opérations sur des blocs
*/
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



/*
    Relatif à la grille
*/
void Refresh_Game(APIGame *game) {
    Refresh_Grid(game);

    // On refresh aussi le next bloc
    Put_Next_Block(game);
}

void Refresh_Grid(APIGame *game) {
    for (int y = 1; y < GAME_API_HEIGHT - 1; y++) {
        for (int x = 1; x < GAME_API_WIDTH - 1; x++) {
            ColorList color_list = Get_Color_List();
            attron(COLOR_PAIR(color_list.colors[game->grid[y][x]]));
            game->grid[y][x] ? mvaddwstr(y, (x - 1) * GAME_WIDTH_MUL + 1, L"[]") : mvaddwstr(y, (x - 1) * GAME_WIDTH_MUL + 1, L"  ");
            attron(COLOR_PAIR(color_list.colors[game->grid[y][x]]));
        }
    }
}



/*
    Relatif au scoring
*/
void Put_Score(int score) {
    // Largeur utile à l'intérieur du bloc (sans les bordures)
    int block_inner_width = NEXT_BLOCK_WIDTH - 2;

    // Début en X du bloc "Next block" (juste après la grille)
    int block_start_x = GAME_WIDTH + 1;

    // Affichage du mot "Score"
    const char *label = "Score";
    int label_x = block_start_x + 1 + (block_inner_width - strlen(label)) / 2;
    mvprintw(7, label_x, "%s", label);

    // Affichage du score
    char score_str[16];
    snprintf(score_str, sizeof(score_str), "%d", score);
    int score_x = block_start_x + 1 + (block_inner_width - strlen(score_str)) / 2;
    attron(COLOR_PAIR(SCORE));
    mvprintw(8, score_x, "%s", score_str);
    attroff(COLOR_PAIR(SCORE));
}

void Put_Level(int level) {
    int y = GAME_API_HEIGHT;
    int x = 1;

    const char *label = "Niveau : ";
    mvprintw(y, x, "%s", label);
    x += strlen(label);

    char level_str[8];
    snprintf(level_str, sizeof(level_str), "%d", level);
    const char *difficulty = Get_Difficulty(level);

    int color_pair = 0;
    if (strcmp(difficulty, "Easy") == 0) {
        color_pair = LEVEL_EASY;
    } else if (strcmp(difficulty, "Medium") == 0) {
        color_pair = LEVEL_MEDIUM;
    } else if (strcmp(difficulty, "Hard") == 0) {
        color_pair = LEVEL_HARD;
    } else if (strcmp(difficulty, "Expert") == 0) {
        color_pair = LEVEL_EXPERT;
    } else if (strcmp(difficulty, "Insane") == 0) {
        color_pair = LEVEL_INSANE;
    }

    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s", level_str);
    x += strlen(level_str);
    mvprintw(y, x, " - %s", difficulty);
    attroff(COLOR_PAIR(color_pair));
}
