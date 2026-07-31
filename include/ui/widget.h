#pragma once

#include <SDL3/SDL.h>
#include "../vec2.h"
#include "../event/event_types.h"

typedef enum WIDGET_TYPE {
    WIDGET_LABEL,
    WIDGET_BUTTON,
    WIDGET_PICBOX,
    WIDGET_CONTAINER,
    WIDGET_SPRITEBOX
} WIDGET_TYPE;

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"
#include "../vec2.h"

typedef struct Widget {
    WIDGET_TYPE wtype;
    vec2 pos;
    int width;
    int height;
    bool visible;
    Event (*notify_func)(struct Widget *self, Event event);
    Event (*input_func)(struct Widget *self, SDL_Event sdl_event);
} Widget;

Event widget_notify(Widget *widget, Event event);
Event widget_input(Widget *widget, SDL_Event sdl_event);