#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

typedef struct GameObject {
    float x;
    float y;
    int width;
    int height;
    bool visible;
} GameObject;

#endif