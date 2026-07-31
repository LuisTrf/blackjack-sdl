#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event.h"

typedef struct SpriteBox SpriteBox;

SpriteBox* spritebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    SDL_Texture *p_spritesheet, 
    int spritesheet_x, int spritesheet_y,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
);
void spritebox_destroy(SpriteBox *p_spritebox);

SDL_Texture* spritebox_get_spritesheet(SpriteBox *spritebox);
int spritebox_get_spritesheet_x(SpriteBox *spritebox);
int spritebox_get_spritesheet_y(SpriteBox *spritebox);
void spritebox_set_spritesheet_x(SpriteBox *spritebox, int spritesheet_x);
void spritebox_set_spritesheet_y(SpriteBox *spritebox, int spritesheet_y);