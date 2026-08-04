#pragma once

#include <SDL3/SDL.h>
#include "../event/event_types.h"

typedef struct InputListener {
    void* self;
    Event (*input_func)(void* self, SDL_Event sdl_event);
} InputListener;