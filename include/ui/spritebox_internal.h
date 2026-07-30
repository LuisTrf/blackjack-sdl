#pragma once

#include "widget_internal.h"
#include "spritebox.h"

typedef struct SpriteBox {
    Widget widget;
    SDL_Texture *p_spritesheet;
    int spritesheet_x;
    int spritesheet_y;
} SpriteBox;