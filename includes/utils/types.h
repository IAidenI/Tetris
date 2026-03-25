#ifndef TYPES_H
#define TYPES_H

typedef enum {
    START,
    RUNNING,
    PAUSED,
    QUIT,
    LOOSE
} GameStatus;

typedef enum {
    INFO,
    WARNING,
    ERROR,
    SUCCESS
} MessageLevel;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int w;
    int h;
} Size;

#endif // TYPES_H