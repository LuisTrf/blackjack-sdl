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
WIDGET_TYPE widget_get_type(Widget* widget);
vec2* widget_get_pos(Widget *widget);
float widget_get_x(Widget *widget);
float widget_get_y(Widget *widget);
int widget_get_width(Widget *widget);
int widget_get_height(Widget *widget);
bool widget_is_visible(Widget *widget);
void widget_set_visibility(Widget *widget, bool visibility);