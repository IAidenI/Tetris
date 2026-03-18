#include "utils/tools.h"

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9; // Get the current time
}

void shuffle_array(void *arr, int n, size_t size) {
    char tmp[size];
    char *a = arr;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        memcpy(tmp,          a + i * size, size);
        memcpy(a + i * size, a + j * size, size);
        memcpy(a + j * size, tmp,          size);
    }
}

void handle_sigint(int sig) {
    (void)sig;
    endwin();
    exit(0);
}

void help(const char* buffer) {
	printf("Usage: %s [OPTION]\n", buffer);
	printf("Une implémentation basique du jeu Tetris en C, jouable dans un terminal.\n\n");
	printf("Pour jouer au jeu il suffit de lancer sans arguments, mais voici quand même quelques options.\n");
	printf("  -d [FICHIER]    Pour fournir un état prédéfinit au jeu\n");
    printf("                  pour facilité le debug.\n");
	printf("  -h, --help      Affiche ce message\n");
	printf("      --version   Affiche la version du script\n");
}

int file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}
