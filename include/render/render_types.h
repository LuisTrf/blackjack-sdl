#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef enum TEXTURE_ID {
    TEXTURE_ID_NULL,
    TEXTURE_ID_BACKGROUND,
    TEXTURE_ID_CARD_SPRITESHEET,
    TEXTURE_ID_DEAL_BUTTON_SPRITESHEET,
    TEXTURE_ID_HIT_BUTTON_SPRITESHEET,
    TEXTURE_ID_STAND_BUTTON_SPRITESHEET,
    TEXTURE_ID_LABEL_DEALER_HAND,
    TEXTURE_ID_LABEL_PLAYER_HAND,
    TEXTURE_ID_LABEL_PLAYER_MONEY
} TEXTURE_ID;

typedef enum FONT_ID {
    FONT_ID_NULL,
    FONT_ID_OPENSANS_32PT,
    FONT_ID_OPENSANS_ITALIC_32PT,
} FONT_ID;

typedef struct texture_hash_t {
    TEXTURE_ID key;
    SDL_Texture* value;
} texture_hash;

typedef struct font_hash_t {
    FONT_ID key;
    TTF_Font* value;
} font_hash;

static const SDL_Color FONT_COLOR = {
    255,
    255,
    255,
    255,
};