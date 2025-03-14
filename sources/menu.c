#include "../headers/menu.h"
#include "../headers/print.h"

int Menu(const int is_loose) {
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

    int choice = 0;
    int key;
    const char *menu[MENU_ITEMS] = { "New Game", "Exit" };
    const char *loose_info = "Perdu !";

    // Calcul position "TETRIS"
    int col_tetris = (GAME_WEIGHT - 6) / 2; // Car TETRIS fait 6 caractères
    int row_tetris = GAME_HEIGHT / 3;

    // Calcul position menu
    int max_lenth = 0;
    for (int i = 0; i < MENU_ITEMS; i++) {
        int len = strlen(menu[i]);
        if (len > max_lenth) {
            max_lenth = len;
        }
    }
    int col_menu = (GAME_WEIGHT - max_lenth) / 2;
    int row_menu = (GAME_HEIGHT / 2) - (MENU_ITEMS / 2);
    
    int col_loose = (GAME_WEIGHT - strlen(loose_info)) / 2;
    int row_loose = row_menu + MENU_ITEMS + 2;

    while (1) {
        clear();

        Create_Frame();

        // Affichage de TETRIS en couleurs
        attron(COLOR_PAIR(RED)); mvprintw(row_tetris, col_tetris, "T"); attroff(COLOR_PAIR(RED));
        attron(COLOR_PAIR(ORANGE)); mvprintw(row_tetris, col_tetris + 1, "E"); attroff(COLOR_PAIR(ORANGE));
        attron(COLOR_PAIR(YELLOW)); mvprintw(row_tetris, col_tetris + 2, "T"); attroff(COLOR_PAIR(YELLOW));
        attron(COLOR_PAIR(GREEN)); mvprintw(row_tetris, col_tetris + 3, "R"); attroff(COLOR_PAIR(GREEN));
        attron(COLOR_PAIR(LIGHT_BLUE)); mvprintw(row_tetris, col_tetris + 4, "I"); attroff(COLOR_PAIR(LIGHT_BLUE));
        attron(COLOR_PAIR(PURPLE)); mvprintw(row_tetris, col_tetris + 5, "S"); attroff(COLOR_PAIR(PURPLE));

        // Affichage du menu
        for (int i = 0; i < MENU_ITEMS; i++) {
            if (i == choice) {
                // Inversion des couleurs
                attron(A_REVERSE);
            }
            mvprintw(row_menu + i, col_menu, "%s", menu[i]);
            attroff(A_REVERSE);
        }

        if (is_loose) {
            attron(COLOR_PAIR(RED));
            mvprintw(row_loose, col_loose, loose_info);
            attroff(COLOR_PAIR(RED));
        }

        key = getch(); // On récuper l'entré utilisateur
        
        if (key == KEY_UP) {
            // Si flêche du haut, on passe à celui au dessus
            choice = (choice - 1 + MENU_ITEMS) % MENU_ITEMS;
        } else if (key == KEY_DOWN) {
            // Si flêche du bas on passe à celui en dessous
            choice = (choice + 1) % MENU_ITEMS;
        } else if (key == '\n') {
            // Si entré on valide
            break;
        } else if (key == 'q') {
            choice = 1;
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
