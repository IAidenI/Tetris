#include "headers/game.h"
#include "headers/menu.h"
#include "headers/print.h"

int main() {
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
