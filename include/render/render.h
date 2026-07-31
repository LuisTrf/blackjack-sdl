#pragma once

#include <SDL3/SDL.h>
#include "../app_state.h"

typedef enum TextureID {
    TEXTURE_ID_NULL,
    TEXTURE_ID_BACKGROUND,
    TEXTURE_ID_CARD_SPRITESHEET,
    TEXTURE_ID_DEAL_BUTTON_SPRITESHEET,
    TEXTURE_ID_HIT_BUTTON_SPRITESHEET,
    TEXTURE_ID_STAND_BUTTON_SPRITESHEET
} TextureID;

typedef struct texture_hash_t {
    TextureID key;
    SDL_Texture* value;
} texture_hash;

texture_hash* texture_map_create(SDL_Renderer *renderer);
void texture_map_destroy(texture_hash* texture_map);
void render(App_State *as);