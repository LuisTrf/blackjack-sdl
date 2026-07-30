#pragma once

#include "animation.h"

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

Animation_Context* animation_context_create(void);
void animation_context_destroy(Animation_Context *p_ac);