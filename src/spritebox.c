#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/spritebox.h"

SpriteBox* spritebox_create(float x, float y, int width, int height, bool visible, SDL_Texture *p_spritesheet, int spritesheet_x, int spritesheet_y,
    Event (*notify_func)(Widget *self, Event event)
){
    SpriteBox spritebox = {{WIDGET_SPRITEBOX, {x, y}, width, height, visible, notify_func}, 
        p_spritesheet, spritesheet_x, spritesheet_y
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