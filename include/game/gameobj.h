#pragma once

#include <stdbool.h>
#include "../vec2.h"

typedef struct GameObject {
    vec2 pos;
    int width;
    int height;
    bool visible;
} GameObject;