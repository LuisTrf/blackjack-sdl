#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event.h"
#include "../render/render.h"

typedef struct SpriteBox {
    Widget widget;
    TEXTURE_ID tid;
    int spritesheet_x;
    int spritesheet_y;
} SpriteBox;

SpriteBox* spritebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    TEXTURE_ID tid, 
    int spritesheet_x, int spritesheet_y
);
void spritebox_destroy(SpriteBox *spritebox);

void spritebox_notify_arrow(void *self, Event event, void *dependencies);