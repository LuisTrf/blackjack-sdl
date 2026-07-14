#pragma once

#include <SDL3/SDL.h>
#include "types.h"

typedef struct render_hash_t {
    i32 key;
    SDL_Texture* value;
} render_hash;

render_hash* texture_map_create(SDL_Renderer *renderer);
void texture_map_destroy(render_hash* texture_map);
void render(SDL_Renderer *renderer, render_hash* texture_map);

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