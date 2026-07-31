#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"

typedef struct Container {
    Widget widget;
    Widget** children;
} Container;

Container* container_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
);
void container_destroy(Container *p_container);

Widget** container_get_children(Container *p_container);

void container_add_widget(Container *p_container, Widget *p_widget);
Widget* container_remove_widget(Container *p_container, Widget *p_widget);