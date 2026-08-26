#pragma once

#include <stdbool.h>
#include "vec2.h"

typedef struct Rect {
    vec2 pos;
    int width;
    int height;
    bool visible;
} Rect;

void rect_align_x(Rect *rect, float target_x);
void rect_align_y(Rect *rect, float target_y);