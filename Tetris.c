#include "headers/game.h"
#include "headers/menu.h"
#include "headers/print.h"
#include "headers/game_info.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0) {
            printf("à faire.\n");
            return 0;
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            Help(argv[0]);
            return 0;
        } else if (strcmp(argv[1], "--version") == 0) {
            printf("%s %s - %s\n", GAME, VERSION, VERSION_DATE);
            printf("  - by %s\n", AUTHOR);
            return 0;
        }
    }

    InitDebug();
    int ret = Menu(0);
    while (ret != EXIT) {
        Debug("Code de retour : %d\n", ret);
        if (ret == ERROR) {
            return 1;
        } else {
            ret = Game();
            if (ret == ERROR) {
                return 1;
            } else if (ret == LOOSE) {
                ret = Menu(1);
            }
        }
    }
    printf("Au revoir.\n");
    return 0;
}
