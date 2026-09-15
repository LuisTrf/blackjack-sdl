#include <stdbool.h>
#include <stdlib.h>
#include "../../include/ui/container.h"
#include "../../include/ui/button.h"
#include "../../vendored/stb/stb_ds.h"

Container* container_create(
    float x, float y, 
    int width, int height, 
    bool visible
){
    Container container = {
        .widget={
            .rect = {
                .pos={.x=x, .y=y}, 
                .width=width, 
                .height=height, 
                .visible=visible, 
            },
            .wtype=WIDGET_CONTAINER, 
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

void moveb_container_notify(void *self, Event event, void* dependencies){
    Container *container = (Container*)self;
    switch(event.type){
        case STATE_EVENT_GAME_STATE:
        case STATE_EVENT_DEAL:
        case STATE_EVENT_HIT:
        case STATE_EVENT_SPLIT_HIT:
        case STATE_EVENT_STAND:
        case STATE_EVENT_SPLIT_STAND:
        case STATE_EVENT_BET:
        case STATE_EVENT_SPLIT:
            reposition_visible_move_buttons(container->children);
            break;
        default:
            break;
    }
}
