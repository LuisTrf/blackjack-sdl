#pragma once

#include <SDL3/SDL.h>
#include "container.h"

typedef enum TextureID {
    TEXTURE_ID_NULL,
    TEXTURE_ID_BACKGROUND,
    TEXTURE_ID_CARD_SPRITESHEET,
    TEXTURE_ID_DEAL_BUTTON_SPRITESHEET,
    TEXTURE_ID_HIT_BUTTON_SPRITESHEET,
    TEXTURE_ID_STAND_BUTTON_SPRITESHEET
} TextureID;

typedef struct render_hash_t {
    TextureID key;
    SDL_Texture* value;
} render_hash;

render_hash* texture_map_create(SDL_Renderer *renderer);
void texture_map_destroy(render_hash* texture_map);
void render(SDL_Renderer *renderer, Container *root);

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