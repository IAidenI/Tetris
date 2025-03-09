#include "headers/game.h"
#include "headers/menu.h"
#include "headers/print.h"

int main(int argc, char **argv) {
    //Init_Blocs();
    int ret = Menu();
    if (ret == -1) {
        return 1;
    } else if (ret == 1) {
        if (argc >= 2) {
            if (strcmp(argv[1], "2") == 0) {
                int ret = Game("None");
                if (ret == -1) {
                    return 1;
                }
            } else {
                int ret = Game(argv[1]);
                if (ret == -1) {
                    return 1;
                }
            }
        } else {
            int ret = Game("None");
            if (ret == -1) {
                return 1;
            }
        }
    } else if (ret == 2) {
        printf("Au revoir.\n");
    }

    return 0;
}
