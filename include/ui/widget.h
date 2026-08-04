#pragma once

#include <SDL3/SDL.h>
#include "../vec2.h"
#include "../rect.h"
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
    Rect rect;
    WIDGET_TYPE wtype;
} Widget;