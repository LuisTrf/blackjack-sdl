#include "../include/rect.h"

void rect_align_x(Rect *rect, float target_x){
    rect->pos.x = target_x - (rect->width)/2.f;
}

void rect_align_y(Rect *rect, float target_y){
    rect->pos.y = target_y - (rect->height)/2.f;
}