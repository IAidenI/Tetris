#include "../headers/menu.h"

int Init_Menu() {
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
    return SUCCESS;
}

int Menu_Pause() {
    const wchar_t *title = L"\u23F8 Pause";
    const char *menu[2] = { "Resume", "Exit" };
    int choice = 0;
    int key;

    int start_row = (GAME_HEIGHT - MENU_PAUSE_HEIGHT) / 2;
    int start_col = (GAME_WIDTH - MENU_PAUSE_WIDTH) / 2;

    int title_row = start_row + 1;
    int title_col = start_col + (MENU_PAUSE_WIDTH - wcslen(title)) / 2;

    int menu_row_start = title_row + 2;

    while (1) {
        // Dessin du cadre
        // Top bordure
        mvaddwstr(start_row, start_col, CORNER_TOP_LEFT);
        for (int j = 1; j < MENU_PAUSE_WIDTH - 1; j++) {
            mvaddwstr(start_row, start_col + j, MIDLESCORE);
        }
        mvaddwstr(start_row, start_col + MENU_PAUSE_WIDTH - 1, CORNER_TOP_RIGHT);
        
        // Milieu
        for (int i = 1; i < MENU_PAUSE_HEIGHT - 1; i++) {
            mvaddwstr(start_row + i, start_col, WALL);
            for (int j = 1; j < MENU_PAUSE_WIDTH - 1; j++) {
                mvaddwstr(start_row + i, start_col + j, L" ");
            }
            mvaddwstr(start_row + i, start_col + MENU_PAUSE_WIDTH - 1, WALL);
        }
        
        // Bottom bordure
        mvaddwstr(start_row + MENU_PAUSE_HEIGHT - 1, start_col, CORNER_BOT_LEFT);
        for (int j = 1; j < MENU_PAUSE_WIDTH - 1; j++) {
            mvaddwstr(start_row + MENU_PAUSE_HEIGHT - 1, start_col + j, MIDLESCORE);
        }
        mvaddwstr(start_row + MENU_PAUSE_HEIGHT - 1, start_col + MENU_PAUSE_WIDTH - 1, CORNER_BOT_RIGHT);

        // Affichage du titre
        mvaddwstr(title_row, title_col, title);

        // Affichage des options du menu
        for (int i = 0; i < 2; i++) {
            int menu_col = start_col + (MENU_PAUSE_WIDTH - strlen(menu[i]) - 4) / 2;
            if (i == choice) {
                mvaddwstr(menu_row_start + i, menu_col, ARROW_LEFT);
                mvprintw(menu_row_start + i, menu_col + 2, "%s", menu[i]);
                mvaddwstr(menu_row_start + i, menu_col + 2 + strlen(menu[i]) + 1, ARROW_RIGHT);
            } else {
                mvprintw(menu_row_start + i, menu_col, "  %s  ", menu[i]);
            }
        }

        key = getch();
        if (key == KEY_UP) {
            choice = (choice - 1 + 2) % 2;
        } else if (key == KEY_DOWN) {
            choice = (choice + 1) % 2;
        } else if (key == '\n') {
            break;
        } else if (key == 'p') {
            choice = 0;
            break;
        }
    }

    return (strcmp(menu[choice], "Resume") == 0) ? SUCCESS : EXIT;
}

int Menu_Confirm() {
    const wchar_t *line1 = L"Are you";
    const wchar_t *line2 = L"sure ?";
    const char *menu[2] = { "Y", "N" };
    int choice = 0;
    int key;

    int start_row = (GAME_HEIGHT - MENU_PAUSE_HEIGHT) / 2;
    int start_col = (GAME_WIDTH - MENU_PAUSE_WIDTH) / 2;

    int line1_row = start_row + 1;
    int line2_row = line1_row + 1;
    int options_row = line2_row + 2;

    while (1) {
        // Bordure haut
        mvaddwstr(start_row, start_col, CORNER_TOP_LEFT);
        for (int j = 1; j < MENU_PAUSE_WIDTH - 1; j++) {
            mvaddwstr(start_row, start_col + j, MIDLESCORE);
        }
        mvaddwstr(start_row, start_col + MENU_PAUSE_WIDTH - 1, CORNER_TOP_RIGHT);

        // Contenu
        for (int i = 1; i < MENU_PAUSE_HEIGHT - 1; i++) {
            mvaddwstr(start_row + i, start_col, WALL);
            for (int j = 1; j < MENU_PAUSE_WIDTH - 1; j++) {
                mvaddwstr(start_row + i, start_col + j, L" ");
            }
            mvaddwstr(start_row + i, start_col + MENU_PAUSE_WIDTH - 1, WALL);
        }

        // Bordure bas
        mvaddwstr(start_row + MENU_PAUSE_HEIGHT - 1, start_col, CORNER_BOT_LEFT);
        for (int j = 1; j < MENU_PAUSE_WIDTH - 1; j++) {
            mvaddwstr(start_row + MENU_PAUSE_HEIGHT - 1, start_col + j, MIDLESCORE);
        }
        mvaddwstr(start_row + MENU_PAUSE_HEIGHT - 1, start_col + MENU_PAUSE_WIDTH - 1, CORNER_BOT_RIGHT);

        // Affichage des deux lignes
        int line1_col = start_col + (MENU_PAUSE_WIDTH - wcslen(line1)) / 2;
        int line2_col = start_col + (MENU_PAUSE_WIDTH - wcslen(line2)) / 2;
        mvaddwstr(line1_row, line1_col, line1);
        mvaddwstr(line2_row, line2_col, line2);

        // Affichage des options avec flèche
        wchar_t options_line[32];
        swprintf(options_line, sizeof(options_line) / sizeof(wchar_t), L"%ls %hs   %ls %hs", (choice == 0) ? ARROW_LEFT : L" ", menu[0], (choice == 1) ? ARROW_LEFT : L" ", menu[1]);


        int options_col = start_col + (MENU_PAUSE_WIDTH - wcslen(options_line)) / 2;
        mvaddwstr(options_row, options_col, options_line);

        // Input
        key = getch();
        if (key == KEY_LEFT) {
            choice = (choice - 1 + 2) % 2;
        } else if (key == KEY_RIGHT) {
            choice = (choice + 1) % 2;
        } else if (key == '\n') {
            break;
        } else if (key == 'q') {
            choice = 1;
        }
    }

    return (choice == 0) ? EXIT : SUCCESS; // Y = 1, N = 0
}

int Menu(const int is_loose) {
    if (Init_Menu()) {
        return ERROR;
    }

    int choice = 0;
    int key;
    const char *menu[MENU_ITEMS] = { "New Game", "Exit" };
    const char *loose_info = "Perdu !";

    int col_tetris = (GAME_WIDTH - 6) / 2;
    int row_tetris = GAME_HEIGHT / 3;

    // Calcul largeur max
    int max_length = 0;
    for (int i = 0; i < MENU_ITEMS; i++) {
        int len = strlen(menu[i]);
        if (len > max_length) {
            max_length = len;
        }
    }

    int row_menu = (GAME_HEIGHT / 2) - (MENU_ITEMS / 2);
    
    int col_loose = (GAME_WIDTH - strlen(loose_info)) / 2;
    int row_loose = row_menu + MENU_ITEMS + 2;

    while (1) {
        clear();

        Create_Frame();

        // Affichage TETRIS en couleurs
        attron(COLOR_PAIR(RED));        mvprintw(row_tetris, col_tetris,     "T"); attroff(COLOR_PAIR(RED));
        attron(COLOR_PAIR(ORANGE));     mvprintw(row_tetris, col_tetris + 1, "E"); attroff(COLOR_PAIR(ORANGE));
        attron(COLOR_PAIR(YELLOW));     mvprintw(row_tetris, col_tetris + 2, "T"); attroff(COLOR_PAIR(YELLOW));
        attron(COLOR_PAIR(GREEN));      mvprintw(row_tetris, col_tetris + 3, "R"); attroff(COLOR_PAIR(GREEN));
        attron(COLOR_PAIR(LIGHT_BLUE)); mvprintw(row_tetris, col_tetris + 4, "I"); attroff(COLOR_PAIR(LIGHT_BLUE));
        attron(COLOR_PAIR(PURPLE));     mvprintw(row_tetris, col_tetris + 5, "S"); attroff(COLOR_PAIR(PURPLE));

        // Affichage du menu avec flèches
        for (int i = 0; i < MENU_ITEMS; i++) {
            wchar_t line[64];
        
            if (i == choice) {
                // ▶ Texte ◀ (avec flèches)
                swprintf(line, sizeof(line) / sizeof(wchar_t), L"\u25B6 %hs \u25C0", menu[i]);
                int line_len = wcslen(line);
                int centered_col = (GAME_WIDTH - line_len) / 2;
        
                mvaddwstr(row_menu + i, centered_col, line);
            } else {
                // Juste le texte centré sans flèches
                int len = strlen(menu[i]);
                int centered_col = (GAME_WIDTH - len) / 2;
                mvprintw(row_menu + i, centered_col, "%s", menu[i]);
            }
        }        

        // Message "Perdu !" si besoin
        if (is_loose) {
            attron(COLOR_PAIR(RED));
            mvprintw(row_loose, col_loose, loose_info);
            attroff(COLOR_PAIR(RED));
        }

        key = getch();
        if (key == KEY_UP) {
            choice = (choice - 1 + MENU_ITEMS) % MENU_ITEMS;
        } else if (key == KEY_DOWN) {
            choice = (choice + 1) % MENU_ITEMS;
        } else if (key == '\n') {
            break;
        } else if (key == 'q') {
            choice = 1; // Exit
            break;
        }
    }

    endwin();

    Debug("Choix utilisateur : menu[%d] = %s\n", choice, menu[choice]);
    if (strcmp(menu[choice], "New Game") == 0) {
        return 1;
    } else if (strcmp(menu[choice], "Exit") == 0) {
        return EXIT;
    } else {
        Error("Ce choix n'existe pas.\n");
        return ERROR;
    }
}


void Help(const char* buffer) {
	printf("Usage: %s [OPTION]\n", buffer);
	printf("Une implémentation basique du jeu Tetris en C, jouable dans un terminal.\n\n");
	printf("Pour jouer au jeu il suffit de lancer sans arguments, mais voici quand même quelques options.\n");
	printf("  -d [FICHIER]    Pour fournir un état prédéfinit au jeu\n");
    printf("                  pour facilité le debug.\n");
	printf("  -h, --help      Affiche ce message\n");
	printf("      --version   Affiche la version du script\n");
}