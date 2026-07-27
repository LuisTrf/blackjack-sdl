#include <stdbool.h>
#include <stdlib.h>
#include "../include/container.h"
#include "../include/spritebox.h"
#include "../include/stb_ds.h"
#include "../include/card_constants.h"
#include "../include/game.h"

#include <stdio.h>

Container* container_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
){
    Container container = {
        .widget={
            .wtype=WIDGET_CONTAINER, 
            .pos={x, y}, 
            .width=width, 
            .height=height, 
            .visible=visible, 
            .notify_func=notify_func,
            .input_func=input_func
        }, 
        .children=NULL
    };
    Container *p_container = malloc(sizeof(Container));
    if (p_container == NULL){
        abort();
    }
    *p_container = container;
    return p_container;
}

void container_destroy(Container *p_container){
    arrfree(p_container->children);
    p_container->children = NULL;
    free(p_container);
    p_container = NULL;
}

Widget** container_get_children(Container *p_container){
    return p_container->children;
}

void container_add_widget(Container *p_container, Widget *p_widget){
    arrput(p_container->children, p_widget);
}

Widget* container_remove_widget(Container *p_container, Widget *p_widget){
    for (int i = 0; i < arrlen(p_container->children); i++){
        if (p_container->children[i] == p_widget){
            arrdel(p_container->children, i);
            return p_widget;
        }
    }
    return NULL;
}

Event container_notify_cards(Widget *self, Event event){
    Container *cself = (Container *)self;
    SpriteBox *tgt_card_spritebox;
    switch (event.type){
        case CARD_EVENT_CARD_DRAWN:
            /*
            Rearrange spriteboxes inside container to render cards in hand such that the newest
            in-hand card is fully visible.
            Partition array into two halves; 
            LHS children[0:51-dcih-pcih] is left alone (in the deck).
            RHS children[52-dcih-pcih:arrlen(children)-1] is in increasing order of when the cards were hit.
            */
            tgt_card_spritebox = (SpriteBox *)cself->children[52 - event.card.dcih - event.card.pcih];
            arrdel(cself->children, 52 - event.card.dcih - event.card.pcih);
            printf("%d, %d\n", event.card.dcih, event.card.pcih);
            arrput(cself->children, (Widget *)tgt_card_spritebox);

            /*
            Consider creating hashmaps for associated i-rank and j-suit pairs.
            */
            for (int i=0; i<13; i++){
                if (event.card.card->rank==ranks[i]){
                    tgt_card_spritebox->spritesheet_x = i*(CARD_WIDTH+2);
                    break;
                }
            }
            for (int j=0; j<4; j++){
                if (event.card.card->suit==suits[j]){
                    tgt_card_spritebox->spritesheet_y = j*(CARD_HEIGHT+2);
                    break;
                }
            }
            if (event.card.card->location == CARD_LOCATION_DEALER_HAND){
                return vec2anim_event_create_with_float_dst_coords(
                    VEC2ANIM_EVENT_CREATE_NEW,
                    &(tgt_card_spritebox->widget.pos),
                    CARD_X_ORIGIN + CARD_STEP_X*(event.card.dcih - 1),
                    CARD_Y_ORIGIN_DEALER
                );
            }
            if (event.card.card->location == CARD_LOCATION_PLAYER_HAND){
                return vec2anim_event_create_with_float_dst_coords(
                    VEC2ANIM_EVENT_CREATE_NEW,
                    &(tgt_card_spritebox->widget.pos),
                    CARD_X_ORIGIN + CARD_STEP_X*(event.card.pcih - 1),
                    CARD_Y_ORIGIN_PLAYER
                );
            }
            break;
        default:
            break;
    }
    return NULL_EVENT;
}