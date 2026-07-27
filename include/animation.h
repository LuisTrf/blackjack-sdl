#pragma once

#include "events.h"
#include "vec2.h"
#include "event_context.h"
#include "update_context.h"

typedef enum AnimationState {
    _ANIMATION_STATE_NONE,
    ANIMATION_STATE_WAITING,
    ANIMATION_STATE_PLAYING,
    ANIMATION_STATE_COMPLETED
} AnimationState;

typedef struct Animation {
    vec2 *target;
    vec2 src;
    vec2 dst;
    AnimationState state;
    Event (*anim_func)(struct Animation *self, float delta_time);
} Animation;

typedef struct AnimationQueue {
    int size;
    int head;
    int tail;
    Animation* arr;
} AnimationQueue;

static const Animation NULL_ANIMATION = {
    NULL,
    {0, 0},
    {0, 0},
    _ANIMATION_STATE_NONE,
    NULL
};

typedef struct AnimationContext AnimationContext;

Animation animation_create(vec2 *target, vec2 dst, Event (*anim_func)(Animation *self, float delta_time));
AnimationQueue* anim_queue_create(int size);
void anim_queue_destroy(AnimationQueue *p_queue);
bool anim_queue_full(AnimationQueue *queue);
bool anim_queue_empty(AnimationQueue *queue);
void enqueue_anim(AnimationQueue *queue, Animation anim);
Animation dequeue_anim(AnimationQueue *queue);
bool anim_is_null(Animation anim);
Event animation_draw_card(Animation *self, float delta_time);
void animate_from_queue(AnimationContext *p_ac, EventContext *p_ec, UpdateContext *p_uc);