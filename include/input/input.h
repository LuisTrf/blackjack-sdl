#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "input_listener.h"
#include "../event/event.h"
#include "../app_state.h"

typedef struct InputContext {
    InputListener* input_listeners;
} InputContext;

InputListener input_listener_create(void* self, Event (*input_func)(void *self, SDL_Event sdl_event));

InputContext* input_context_create(void);
void input_context_destroy(InputContext *input_ctx);
void input_ctx_listener_register(InputContext *input_ctx, InputListener input_listener);
void input_ctx_listener_remove(InputContext *input_ctx, InputListener input_listener);

bool input_handle(AppState *as);

Event input_handle_button_mouse_events(void *self, SDL_Event sdl_event);