#pragma once

#include "../event/event.h"
#include "../vec2.h"

#define ANIMATION_POOL_MAXIMUM_ANIMATIONS 16

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
    int frame_count;
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

AnimationQueue* anim_queue_create(int size);
void anim_queue_destroy(AnimationQueue *p_queue);
bool anim_queue_full(AnimationQueue *queue);
bool anim_queue_empty(AnimationQueue *queue);
void anim_enqueue(AnimationQueue *queue, Animation anim);

AnimationPool* anim_pool_create(int size);
void anim_pool_destroy(AnimationPool *anim_pool);
int anim_add(AnimationPool *anim_pool, Animation anim);

bool anim_is_null(Animation anim);

Event animation_draw_card(Animation *self, float delta_time);
Event animation_cheque_move(Animation *self, float delta_time);

void animate(AnimationQueue *anim_queue, AnimationPool *anim_pool, EventQueue *event_queue, float delta_time);