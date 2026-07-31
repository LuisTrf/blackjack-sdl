#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/ui/picbox.h"

PictureBox* picturebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    SDL_Texture *texture,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
){
    PictureBox picbox = {
        {
            WIDGET_PICBOX, 
            {x, y}, 
            width, 
            height, 
            visible, 
            notify_func,
            input_func
        }, 
        texture
    };
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