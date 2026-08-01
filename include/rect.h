#pragma once

#include "vec2.h"

typedef struct Rect {
    vec2 pos;
    int width;
    int height;
    bool visible;
} Rect;