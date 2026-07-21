#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"

typedef struct PictureBox {
    Widget widget;
    SDL_Texture *p_texture;
} PictureBox;

PictureBox* picturebox_create(float x, float y, int width, int height, bool visible, SDL_Texture *p_texture, 
    void (*update_func)(Widget *self, Event event)
);
void picturebox_destroy(PictureBox *p_picture_box);