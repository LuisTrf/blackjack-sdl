#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"
#include "../render/render_types.h"

typedef struct PictureBox {
    Widget widget;
    TEXTURE_ID tid;
} PictureBox;

PictureBox* picturebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    TEXTURE_ID tid, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event sdl_event)
);
void picturebox_destroy(PictureBox *picturebox);