#pragma once

#include "animation_types.h"
#include "../main.h"

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
Event animation_betting_elements_move(Animation *self, float delta_time);
Event animation_arrow(Animation *self, float delta_time);

void animate(AppState *as);