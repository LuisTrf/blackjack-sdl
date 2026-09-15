#include <stdlib.h>
#include <stdbool.h>
#include "../../include/ui/spritebox_constants.h"
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

void spritebox_notify_arrow(void *self, Event event, void *dependencies){
    SpriteBox *spritebox = (SpriteBox *)self;
    switch (event.type){
        case STATE_EVENT_DEAL:
            rect_vis_set((Rect *)spritebox, false);
            break;
        case STATE_EVENT_SPLIT:
            rect_vis_set((Rect *)spritebox, true);
            break;
        case STATE_EVENT_GAME_STATE: {
            GAME_STATE game_state = event.state.data.game_state.game_state;
            switch (game_state) {
                case GAME_STATE_BETTING_PLAYING:
                    spritebox->widget.rect.pos = (vec2){ARROW_HAND_X, ARROW_HAND_Y};
                    break;
                case GAME_STATE_PLAYING_SPLIT:
                    spritebox->widget.rect.pos = (vec2){ARROW_HAND_X, ARROW_SPLIT_HAND_Y};
                    break;
                case GAME_STATE_FIN:
                    rect_vis_set((Rect *)spritebox, false);
                    break;
                default:    
                    break;
            }
            break;
        }
        case ANIMATION_EVENT_QUEUE_BLOCKING: 
            rect_hide((Rect *)spritebox);
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            rect_unhide((Rect *)spritebox);
            break;
        default:
            break;
    }
}