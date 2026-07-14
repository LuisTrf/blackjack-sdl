#pragma once

#include <SDL3/SDL.h>
#include "container.h"

typedef struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Container *root;
} AppState;