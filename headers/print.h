#ifndef PRINT_H
#define PRINT_H

#define Error(text, ...) fprintf(stderr, "Erreur: " text, ##__VA_ARGS__)
#define Debug(text, ...) fprintf(stderr, "[%s] : " text, __func__ , ##__VA_ARGS__); fflush(stdout);

#endif
