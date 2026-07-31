#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"

typedef struct PictureBox {
    Widget widget;
    SDL_Texture *p_texture;
} PictureBox;

PictureBox* picturebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    SDL_Texture *p_texture, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
);
void picturebox_destroy(PictureBox *p_picture_box);