#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "../ui/widget.h"
#include "../event/event.h"
#include "../app_state.h"

typedef struct InputContext {
    Widget** input_widget_listeners;
} InputContext;

InputContext* input_context_create(void);
void input_context_destroy(InputContext *input_ctx);
void input_ctx_widget_listener_register(InputContext *input_ctx, Widget *widget);
void input_ctx_widget_listener_remove(InputContext *input_ctx, Widget *widget);
bool input_handle(AppState *as);
Event input_handle_button_mouse_events(Widget *button, SDL_Event sdl_event);