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
    Event (*input_func)(Widget *self, SDL_Event sdl_event)
);
void container_destroy(Container *container);

Widget** container_get_children(Container *container);

void container_add_widget(Container *container, Widget *widget);
Widget* container_remove_widget(Container *container, Widget *widget);