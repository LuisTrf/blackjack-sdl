#include "../include/rect.h"

void rect_align_x(Rect *rect, float target_x){
    rect->pos.x = target_x - (rect->width)/2.f;
}

void rect_align_y(Rect *rect, float target_y){
    rect->pos.y = target_y - (rect->height)/2.f;
}

void rect_vis_set(Rect *rect, bool visible){
    rect->prev_visibility = rect->visible;
    rect->visible = visible;
}

void rect_hide(Rect *rect){
    if (rect->visible || rect->prev_visibility){
        rect_vis_set(rect, false);
    }
}

void rect_unhide(Rect *rect){
    if (rect->prev_visibility && !rect->visible){
        rect_vis_set(rect, true);
    }
}