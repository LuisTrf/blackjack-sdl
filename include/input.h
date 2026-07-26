#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "button.h"

typedef struct InputContext {
    Widget** widget_listeners;
} InputContext;

InputContext* input_context_initialize(void);
void input_context_teardown(InputContext *p_ic);
void input_context_widget_listener_add(InputContext *p_ic, Widget *widget);
void input_context_widget_listener_remove(InputContext *p_ic, Widget *widget);
bool handle_input(InputContext *p_ic, struct EventQueue *event_queue);
Event input_handle_button_mouse_events(Button *button, SDL_Event event);