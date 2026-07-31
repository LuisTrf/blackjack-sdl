#pragma once

#include "../event/event.h"
#include "../vec2.h"
#include "../app_state.h"

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

static const Animation NULL_ANIMATION = {
    NULL,
    {0, 0},
    {0, 0},
    _ANIMATION_STATE_NONE,
    NULL
};

typedef struct AnimationQueue {
    int size;
    int head;
    int tail;
    Animation* arr;
} AnimationQueue;

typedef struct Animation_Context {
    AnimationQueue *queue;
    Animation *playing_blocking_anim;
} Animation_Context;

Animation animation_create(vec2 *target, vec2 dst, Event (*anim_func)(Animation *self, float delta_time));
AnimationQueue* anim_queue_create(int size);
void anim_queue_destroy(AnimationQueue *p_queue);
bool anim_queue_full(AnimationQueue *queue);
bool anim_queue_empty(AnimationQueue *queue);
void enqueue_anim(AnimationQueue *queue, Animation anim);
Animation dequeue_anim(AnimationQueue *queue);
bool anim_is_null(Animation anim);
Event animation_draw_card(Animation *self, float delta_time);

Animation_Context* animation_context_create(void);
void animation_context_destroy(Animation_Context *p_ac);

void animate_from_queue(Animation_Context *p_ac, Event_Context *p_ec, float delta_time);
void animate(App_State *as);