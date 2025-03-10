#include <ncurses.h>
#include <unistd.h> // Pour sleep()

int main() {
    initscr();
    start_color();
    curs_set(0);
    noecho();

    // Définir les paires de couleurs (texte noir, fond rouge et inversement)
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);

    int toggle = 0;
    for (int i = 0; i < 10; i++) { // Clignote 10 fois
        attron(COLOR_PAIR(toggle ? 1 : 2));
        mvprintw(LINES / 2, (COLS - 20) / 2, " FOND ROUGE CLIGNOTANT ");
        attroff(COLOR_PAIR(toggle ? 1 : 2));

        refresh();
        usleep(300000); // Pause de 300ms pour l'effet de clignotement
        toggle = !toggle; // Alterner la couleur
    }

    getch();
    endwin();
    return 0;
}
