#pragma once

#include <stdbool.h>
#include "widget.h"
#include "../render/render_types.h"

typedef struct PictureBox {
    Widget widget;
    TEXTURE_ID tid;
} PictureBox;

PictureBox* picturebox_create(
    float x, float y, 
    int width, int height, 
    int sprite_width, int sprite_height,
    bool visible, 
    TEXTURE_ID tid
);
void picturebox_destroy(PictureBox *picturebox);