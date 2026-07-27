#pragma once

#include "animation.h"

typedef struct AnimationContext {
    AnimationQueue *queue;
    Animation *playing_blocking_anim;
} AnimationContext;

AnimationContext* animation_context_create(void);
void animation_context_destroy(AnimationContext *p_ac);