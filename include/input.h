#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "button.h"
#include "event_context.h"

typedef struct InputContext {
    Widget** input_widget_listeners;
} InputContext;

InputContext* input_context_create(void);
void input_context_destroy(InputContext *p_ic);
void ic_widget_listener_register(InputContext *p_ic, Widget *widget);
void ic_widget_listener_remove(InputContext *p_ic, Widget *widget);
bool input_handle(InputContext *p_ic, EventQueue *event_queue);
Event input_handle_button_mouse_events(Widget *button, SDL_Event event);