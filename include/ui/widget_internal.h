#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event.h"
#include "../vec2.h"

typedef struct Widget {
    WIDGET_TYPE wtype;
    vec2 pos;
    int width;
    int height;
    bool visible;
    Event (*notify_func)(struct Widget *self, Event event);
    Event (*input_func)(struct Widget *self, SDL_Event event);
} Widget;