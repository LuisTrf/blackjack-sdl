#include <stdlib.h>
#include <stdbool.h>
#include "../../include/ui/picbox.h"

PictureBox* picturebox_create(
    float x, float y, 
    int width, int height, 
    int sprite_width, int sprite_height, 
    bool visible, 
    TEXTURE_ID tid
){
    PictureBox picbox = {
        {
            {
                .pos={.x=x, .y=y}, 
                .width=width, 
                .height=height, 
                .sprite_width=sprite_width,
                .sprite_height=sprite_height,
                .visible=visible, 
            },
            WIDGET_PICBOX, 
        }, 
        tid
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