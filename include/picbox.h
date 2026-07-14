#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"

typedef struct PictureBox {
    Widget widget;
    SDL_Texture *p_texture;
} PictureBox;

PictureBox* picturebox_create(f32 x, f32 y, i32 width, i32 height, bool visible, SDL_Texture *p_texture, void (*update_func)(Widget *self, Event event));
void picturebox_destroy(PictureBox *p_picture_box);