#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "input_listener.h"
#include "../event/event.h"
#include "../main.h"

void input_listener_register(InputListener **input_listeners, InputListener input_listener);
void input_listener_remove(InputListener **input_listeners, InputListener input_listener);

bool input_handle(AppState *as);

Event input_handle_button_mouse_events(void *self, SDL_Event sdl_event);