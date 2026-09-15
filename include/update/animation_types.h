#pragma once

#include "animation_constants.h"
#include "../event/event_types.h"
#include "../rect.h"

typedef enum ANIMATION_TYPE {
    _ANIMATION_TYPE_NONE,
    ANIMATION_TYPE_VEC2,
    ANIMATION_TYPE_SPRITESHEET
} ANIMATION_TYPE;

typedef enum ANIMATION_STATE {
    _ANIMATION_STATE_NONE,
    ANIMATION_STATE_WAITING,
    ANIMATION_STATE_PLAYING,
    ANIMATION_STATE_COMPLETED
} ANIMATION_STATE;

typedef struct Vec2Animation {
    vec2 src;
    vec2 dst;
} Vec2Animation;

typedef struct SpriteAnimation {
    int frame_idx;
    float ttnf;
    int fps;
    int sheet_step_x;
} SpriteAnimation;

typedef struct Animation {
    Rect *target;
    ANIMATION_TYPE type;
    ANIMATION_STATE state;
    Event (*anim_func)(struct Animation *self, float delta_time);
    union {
        Vec2Animation vec2_anim;
        SpriteAnimation sprite_anim;
    };
} Animation;

static const Animation NULL_ANIMATION = {
    NULL,
    _ANIMATION_TYPE_NONE,
    _ANIMATION_STATE_NONE,
    NULL,
};

typedef struct AnimationQueue {
    int size;
    int count;
    int head;
    int tail;
    bool queue_is_blocking;
    Animation *playing_blocking_anim;
    Animation *arr;
} AnimationQueue;

typedef struct AnimationPool {
    Animation arr[ANIMATION_POOL_MAXIMUM_ANIMATIONS];
    int free_list[ANIMATION_POOL_MAXIMUM_ANIMATIONS];
    int size;
    int free_count;
} AnimationPool;