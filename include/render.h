#pragma once

#include <SDL3/SDL.h>
#include "types.h"

typedef struct render_hash_t {
    i32 key;
    SDL_Texture* value;
} render_hash;

typedef struct RenderState {
    SDL_Renderer *renderer;
    render_hash* texture_map;
} RenderState;

RenderState* render_initialize(SDL_Renderer *sdl_renderer);
void render_teardown(RenderState *rs);
void render(RenderState *rs);

/*
#define SPRITESHEET_SEP 2
#define BUTTON_SPRITESHEET_IDLE_REL_OFFSET_X 0
#define BUTTON_SPRITESHEET_IDLE_REL_OFFSET_Y 0
#define BUTTON_SPRITESHEET_DISABLED_REL_OFFSET_X 1
#define BUTTON_SPRITESHEET_DISABLED_REL_OFFSET_Y 0
#define BUTTON_SPRITESHEET_HOVERED_REL_OFFSET_X 0
#define BUTTON_SPRITESHEET_HOVERED_REL_OFFSET_Y 1
#define BUTTON_SPRITESHEET_PRESSED_REL_OFFSET_X 1
#define BUTTON_SPRITESHEET_PRESSED_REL_OFFSET_Y 1
*/