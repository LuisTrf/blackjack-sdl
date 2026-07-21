#pragma once

#include "widget.h"

typedef struct Container {
    Widget widget;
    Widget** children;
} Container;

Container* container_create(float x, float y, int width, int height, bool visible, 
    void (*update_func)(Widget *self, Event event)
);
void container_destroy(Container *p_container);
Widget** container_get_children(Container *p_container);
void container_add_widget(Container *p_container, Widget *p_widget);
Widget* container_remove_widget(Container *p_container, Widget *p_widget);