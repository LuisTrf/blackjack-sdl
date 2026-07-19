#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "button.h"

typedef struct InputContext {
    Widget** widget_subscribers;
} InputContext;

InputContext* input_initialize(void);
void input_teardown(InputContext *p_ic);
void input_subscriber_add(InputContext *p_ic, Widget *widget);
void input_subscriber_remove(InputContext *p_ic, Widget *widget);
bool handle_input(InputContext *p_ic);
void button_handle_mouse_events(Button *button, SDL_Event event);