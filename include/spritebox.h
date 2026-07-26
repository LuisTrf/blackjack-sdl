#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"

typedef struct SpriteBox {
    Widget widget;
    SDL_Texture *p_spritesheet;
    int spritesheet_x;
    int spritesheet_y;
} SpriteBox;

SpriteBox* spritebox_create(float x, float y, int width, int height, bool visible, SDL_Texture *p_spritesheet, int spritesheet_x, int spritesheet_y,
    Event (*notify_func)(Widget *self, Event event));
void spritebox_destroy(SpriteBox *p_spritebox);