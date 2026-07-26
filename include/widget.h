#pragma once

#include <stdbool.h>
#include "vec2.h"
#include "events.h"

typedef enum {
    WIDGET_LABEL,
    WIDGET_BUTTON,
    WIDGET_PICBOX,
    WIDGET_CONTAINER,
    WIDGET_SPRITEBOX
} WIDGET_TYPE;

typedef struct Widget {
    WIDGET_TYPE wtype;
    vec2 pos;
    int width;
    int height;
    bool visible;
    Event (*notify_func)(struct Widget *self, Event event);
} Widget;