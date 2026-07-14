#include <stdbool.h>
#include <stdlib.h>
#include "../include/types.h"
#include "../include/container.h"
#include "../include/stb_ds.h"

Container* container_create(f32 x, f32 y, i32 width, i32 height, bool visible, void (*update_func)(Widget *self, Event event)){
    Container container = {{WIDGET_CONTAINER, {x, y}, width, height, visible, update_func}, NULL};
    Container *p_container = malloc(sizeof(Container));
    if (p_container == NULL){
        abort();
    }
    arrsetlen(container.children, CONTAINER_INITIAL_CHILDREN);
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
    for (i32 i = 0; i < arrlen(p_container->children); i++){
        if (p_container->children[i] == p_widget){
            arrdel(p_container->children, i);
            return p_widget;
        }
    }
    return NULL;
}