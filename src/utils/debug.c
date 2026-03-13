#include "utils/debug.h"

static const char *log_path = NULL;

void log_init(const char *path) {
    log_path = path;
}

void log_write(char *message, ...) {
    if (log_path == NULL) {
        printf("[!] log_init not called.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(log_path, "a");
    if (!fp) return;

    va_list args;
    va_start(args, message);
    vfprintf(fp, message, args);
    va_end(args);

    fclose(fp);
}
