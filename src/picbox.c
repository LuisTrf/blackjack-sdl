#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/types.h"
#include "../include/picbox.h"

PictureBox* picturebox_create(f32 x, f32 y, i32 width, i32 height, bool visible, SDL_Texture *p_texture,
    void (*update_func)(Widget *self, Event event)
){
    PictureBox picbox = {{WIDGET_PICBOX, {x, y}, width, height, visible, update_func}, p_texture};
    PictureBox *p_picbox = malloc(sizeof(PictureBox));
    if (p_picbox==NULL){
        abort();
    }
    *p_picbox=picbox;
    return p_picbox;
}

void picturebox_destroy(PictureBox *p_picbox){
    free(p_picbox);
    p_picbox=NULL;
}