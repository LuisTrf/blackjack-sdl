#include <stdbool.h>
#include <stdlib.h>
#include "../../include/ui/container_internal.h"
#include "../../include/stb_ds.h"

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