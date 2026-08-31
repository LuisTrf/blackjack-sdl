#pragma once

#include <stdbool.h>
#include "vec2.h"

typedef struct Rect {
    vec2 pos;
    int width;
    int height;
    bool visible;
    bool prev_visibility;
} Rect;

void rect_align_x(Rect *rect, float target_x);
void rect_align_y(Rect *rect, float target_y);
void rect_vis_set(Rect *rect, bool visible);
void rect_hide(Rect *rect);
void rect_unhide(Rect *rect);