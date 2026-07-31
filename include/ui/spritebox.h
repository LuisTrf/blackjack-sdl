#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event.h"

typedef struct SpriteBox {
    Widget widget;
    SDL_Texture *spritesheet;
    int spritesheet_x;
    int spritesheet_y;
} SpriteBox;

SpriteBox* spritebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    SDL_Texture *spritesheet, 
    int spritesheet_x, int spritesheet_y,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event sdl_event)
);
void spritebox_destroy(SpriteBox *spritebox);