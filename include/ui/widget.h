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

typedef struct Widget Widget;

Event widget_notify(Widget *widget, Event event);
Event widget_input(Widget *widget, SDL_Event event);