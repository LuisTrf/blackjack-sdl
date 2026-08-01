#pragma once

#include "../event/event.h"
#include "../vec2.h"
#include "../cargo.h"
#include "../app_state.h"

typedef enum ANIMATION_STATE {
    _ANIMATION_STATE_NONE,
    ANIMATION_STATE_WAITING,
    ANIMATION_STATE_PLAYING,
    ANIMATION_STATE_COMPLETED
} ANIMATION_STATE;

typedef struct Animation {
    Rect *target;
    vec2 src;
    vec2 dst;
    ANIMATION_STATE state;
    Event (*anim_func)(struct Animation *self, float delta_time);
    Cargo cargo;
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

typedef struct AnimationContext {
    AnimationQueue *queue;
    Animation *playing_blocking_anim;
} AnimationContext;

Animation animation_create(Rect *target, vec2 dst, Event (*anim_func)(Animation *self, float delta_time), Cargo cargo);
AnimationQueue* anim_queue_create(int size);
void anim_queue_destroy(AnimationQueue *p_queue);
bool anim_queue_full(AnimationQueue *queue);
bool anim_queue_empty(AnimationQueue *queue);
void anim_enqueue(AnimationQueue *queue, Animation anim);
Animation anim_dequeue(AnimationQueue *queue);
bool anim_is_null(Animation anim);

Event animation_draw_card(Animation *self, float delta_time);

AnimationContext* animation_context_create(void);
void animation_context_destroy(AnimationContext *anim_ctx);

void animate_from_queue(AnimationContext *anim_ctx, EventContext *event_ctx, float delta_time);
void animate(AppState *as);