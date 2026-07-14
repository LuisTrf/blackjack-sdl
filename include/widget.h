#pragma once

#include <stdbool.h>
#include "types.h"
#include "vec2.h"

typedef enum {
    WIDGET_LABEL,
    WIDGET_BUTTON,
    WIDGET_PICBOX,
    WIDGET_CONTAINER,
    _WIDGET_TYPE_COUNT
} WIDGET_TYPE;

typedef enum {
    EVENT_EVENT
} Event;

typedef struct Widget {
    WIDGET_TYPE wtype;
    vec2 pos;
    i32 width;
    i32 height;
    bool visible;
    void (*update_func)(struct Widget *self, Event event);
} Widget;