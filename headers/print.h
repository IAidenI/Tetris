#ifndef PRINT_H
#define PRINT_H

#define Error(text, ...) fprintf(stderr, "Erreur: " text, ##__VA_ARGS__)
#define Debug(text, ...) printf("[DEBUG] " text, ##__VA_ARGS__)

#endif
