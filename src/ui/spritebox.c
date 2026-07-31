#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/ui/spritebox_internal.h"

SpriteBox* spritebox_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    SDL_Texture *p_spritesheet, 
    int spritesheet_x, int spritesheet_y,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
){
    SpriteBox spritebox = {
        {
            WIDGET_SPRITEBOX, 
            {x, y}, 
            width, 
            height, 
            visible, 
            notify_func,
            input_func
        }, 
        p_spritesheet, 
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

SDL_Texture* spritebox_get_spritesheet(SpriteBox *spritebox){return spritebox->p_spritesheet;}
int spritebox_get_spritesheet_x(SpriteBox *spritebox){return spritebox->spritesheet_x;}
int spritebox_get_spritesheet_y(SpriteBox *spritebox){return spritebox->spritesheet_y;}
void spritebox_set_spritesheet_x(SpriteBox *spritebox, int spritesheet_x){spritebox->spritesheet_x = spritesheet_x;}
void spritebox_set_spritesheet_y(SpriteBox *spritebox, int spritesheet_y){spritebox->spritesheet_y = spritesheet_y;}