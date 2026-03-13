#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void log_init(const char *path);
void log_write(char *message, ...);

#endif // DEBUG_H
