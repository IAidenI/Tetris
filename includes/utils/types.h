#ifndef TYPES_H
#define TYPES_H

typedef enum {
    SNAPSHOT,
    START,
    RUNNING,
    PAUSED,
    QUIT,
    LOOSE
} GameStatus;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int w;
    int h;
} Size;

#endif // TYPES_H