#pragma once

#include "widget_internal.h"
#include "picbox.h"

typedef struct PictureBox {
    Widget widget;
    SDL_Texture *p_texture;
} PictureBox;