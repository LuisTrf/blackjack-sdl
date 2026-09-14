#pragma once

#include "../vendored/SDL_mixer/include/SDL3_mixer/SDL_mixer.h"

typedef enum AUDIO_ID {
    AUDIO_ID_CARD_PLACE_1,
    AUDIO_ID_CARD_SLIDE_1,
    AUDIO_ID_CHIP_LAY_1,
    AUDIO_ID_OWIN31,
    AUDIO_ID_WOMP_WOMP,
} AUDIO_ID;

typedef struct audio_hash_t {
    AUDIO_ID key;
    MIX_Audio* value;
} audio_hash;

typedef enum TRACK_ID {
    TRACK_ID_CARD_DEAL,
    TRACK_ID_CARD_FLIP,
    TRACK_ID_CHEQUE_DEAL,
    TRACK_ID_BLACKJACK_WIN,
    TRACK_ID_BLACKJACK_LOSS,
} TRACK_ID;

typedef struct track_hash_t {
    TRACK_ID key;
    MIX_Track* value;
} track_hash;