#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/ui/spritebox.h"

SpriteBox* spritebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    TEXTURE_ID tid, 
    int spritesheet_x, int spritesheet_y
){
    SpriteBox spritebox = {
        {
            .rect = {
                .pos={.x=x, .y=y}, 
                .width=width, 
                .height=height, 
                .visible=visible, 
            },
            WIDGET_SPRITEBOX,
        }, 
        tid, 
        spritesheet_x, 
        spritesheet_y
    };
    SpriteBox *p_spritebox = malloc(sizeof(SpriteBox));
    if (p_spritebox==NULL){
        abort();
    }
    *p_spritebox=spritebox;
    return p_spritebox;
}

void spritebox_destroy(SpriteBox *p_spritebox){
    free(p_spritebox);
    p_spritebox=NULL;
}