#include "headers/game.h"
#include "headers/menu.h"
#include "headers/print.h"

int main() {
    //Init_Blocs();
    int ret = Menu();
    if (ret == -1) {
        return 1;
    } else if (ret == 1) {
        int ret = Game();
        if (ret == -1) {
            return 1;
        }
    } else if (ret == 2) {
        printf("Au revoir.\n");
    }

    return 0;
}
