#pragma once

#include "widget_internal.h"
#include "label.h"

typedef struct Label {
    Widget widget;
    TTF_Font *p_font;
    SDL_Texture *p_texture;
    float font_size;
    char txt[32];
} Label;