#include "tools.h"

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
