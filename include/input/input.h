#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "../ui/widget.h"
#include "../event/event.h"
#include "../app_state.h"

typedef struct Input_Context {
    Widget** input_widget_listeners;
} Input_Context;

Input_Context* input_context_create(void);
void input_context_destroy(Input_Context *p_ic);
void ic_widget_listener_register(Input_Context *p_ic, Widget *widget);
void ic_widget_listener_remove(Input_Context *p_ic, Widget *widget);
bool input_handle(App_State *as);
Event input_handle_button_mouse_events(Widget *button, SDL_Event event);