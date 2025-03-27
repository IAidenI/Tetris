#include "headers/game.h"
#include "headers/menu.h"
#include "headers/print.h"
#include "headers/game_info.h"

int main(int argc, char **argv) {
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
    InitDebug();

    APIGame game;

    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0) {
            if (argc < 3) {
                Error("Il faut spécifier un nom de fichier. (Voir %s --help)\n", argv[0]);
                return 1;
            }

            if (Start_Game(&game)) {
                return 1;
            }
            Debug("APIGame initialisé.\n");
            Display_Grid("On a initialiser la grille :", &game);
            
            if (Set_Game(&game, argv[2])) {
                Stop_Game(&game);
                return 1;
            }
            game.flag = FLAG_DEBUG;
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            Help(argv[0]);
            return 0;
        } else if (strcmp(argv[1], "--version") == 0) {
            printf("%s %s - %s\n", GAME, VERSION, VERSION_DATE);
            printf("  - by %s\n", AUTHOR);
            return 0;
        } else {
            Help(argv[0]);
            return 0;
        }
    }

    
    int ret = game.flag == FLAG_DEBUG ? 0 : Menu(0);
    while (ret != EXIT) {
        Debug("Code de retour : %d\n", ret);

        if (ret == ERROR) {
            return 1;
        }

        if (game.flag != FLAG_DEBUG) {
            if (Start_Game(&game)) {
                Error("Impossible de redémarrer le jeu.\n");
                return 1;
            }
        }

        ret = Game(&game);
        Stop_Game(&game);

        ret = (ret == LOOSE) ? Menu(1) : Menu(0);
    }
    
    printf("Au revoir.\n");
    return 0;
}
