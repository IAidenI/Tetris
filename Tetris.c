#include "headers/game.h"
#include "headers/menu.h"
#include "headers/print.h"

int main(int argc, char **argv) {
    //Init_Blocs();
    int ret = Menu(0);
    while (ret != EXIT) {
        Debug("ret %d\n", ret);
        if (ret == ERROR) {
            return 1;
        } else {
            if (argc >= 2) {
                if (strcmp(argv[1], "2") == 0) {
                    ret = Game("None");
                    if (ret == ERROR) {
                        return 1;
                    } else if (ret == LOOSE) {
                        ret = Menu(1);
                    }
                } else {
                    ret = Game(argv[1]);
                    if (ret == ERROR) {
                        return 1;
                    } else if (ret == LOOSE) {
                        ret = Menu(1);
                    }
                }
            } else {
                ret = Game("None");
                if (ret == ERROR) {
                    return 1;
                } else if (ret == LOOSE) {
                    ret = Menu(1);
                    Debug("????? : %d\n", ret);
                }
            }
         }
    }
    printf("Au revoir.\n");
    return 0;
}
