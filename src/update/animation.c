#include <stdlib.h>
#include <stdio.h>
#include "../../include/update/animation.h"

Animation animation_create(vec2 *target, vec2 dst, Event (*anim_func)(Animation *self, float delta_time)){
    Animation anim = {
        .target = target,
        .src = *target,
        .dst = dst,
        .state = ANIMATION_STATE_WAITING,
        .anim_func = anim_func,
    };
    return anim;
}

AnimationQueue* anim_queue_create(int size){
    Animation* arr = calloc(size, sizeof(Animation));
    if (arr == NULL){
        abort();
    }
    AnimationQueue queue = {.size=size, .head=0, .tail=0, .arr=arr};
    AnimationQueue *p_queue = malloc(sizeof(queue));
    if (p_queue == NULL){
        abort();
    }
    *p_queue = queue;
    return p_queue;
}

void anim_queue_destroy(AnimationQueue *p_queue){
    free(p_queue->arr);
    p_queue->arr = NULL;
    free(p_queue);
}

Animation_Context* animation_context_create(void){
    AnimationQueue *queue = anim_queue_create(16);
    Animation *p_anim = malloc(sizeof(Animation));
    if (p_anim == NULL){
        abort();
    }
    *p_anim = NULL_ANIMATION;
    Animation_Context ac = {queue, p_anim};
    Animation_Context *p_ac = malloc(sizeof(Animation_Context));
    if (p_ac == NULL){
        abort();
    }
    *p_ac = ac;
    return p_ac;
}

void animation_context_destroy(Animation_Context *p_ac){
    free(p_ac->playing_blocking_anim);
    p_ac->playing_blocking_anim = NULL;
    anim_queue_destroy(p_ac->queue);
    p_ac->queue = NULL;
    free(p_ac);
}

bool anim_queue_full(AnimationQueue *queue){
    if (
        queue->head==queue->tail+1 
        || (queue->head==0 && queue->tail==queue->size-1)
    ){
        return true;
    }
    else {
        return false;
    }
}

bool anim_queue_empty(AnimationQueue *queue){
    if (queue->head==queue->tail) {
        return true;
    }
    else {
        return false;
    }
}

void enqueue_anim(AnimationQueue *queue, Animation anim){
    if (anim_queue_full(queue)){
        fprintf(stderr, "ANIM QUEUE OVERFLOW!");
        return;
    }
    else{
        queue->arr[queue->tail] = anim;
        if (queue->tail==queue->size-1){
            queue->tail=0;
        }
        else {
            queue->tail++;
        }
    }
}

Animation dequeue_anim(AnimationQueue *queue){
    if (anim_queue_empty(queue)){
        fprintf(stderr, "ANIM QUEUE UNDERFLOW!");
        return NULL_ANIMATION;
    }
    else {
        Animation a = queue->arr[queue->head];
        if (queue->head==queue->size-1){
            queue->head=0;
        }
        else {
            queue->head++;
        }
        return a;
    }
}

bool anim_is_null(Animation anim){
    if (
        anim.target == NULL_ANIMATION.target
        && anim.dst.x == NULL_ANIMATION.dst.x
        && anim.dst.y == NULL_ANIMATION.dst.y
        && anim.src.x == NULL_ANIMATION.src.x
        && anim.src.y == NULL_ANIMATION.src.y
        && anim.state == NULL_ANIMATION.state
        && anim.anim_func == NULL_ANIMATION.anim_func
    ){
        return true;
    }
    else {
        return false;
    }
}

void vec2_translate_in_fixed_time(float delta_time, vec2 *target, vec2 src, vec2 dst, float time){
    if (target->x > dst.x){
        if ((target->x - dst.x) < (delta_time/time) * (src.x - dst.x)){
            target->x = dst.x;
        }
        else {
            target->x -= (delta_time/time) * (src.x - dst.x);
        }
    }
    else if (target->x < dst.x){
        if ((dst.x - target->x) < (delta_time/time) * (dst.x - src.x)){
            target->x = dst.x;
        }
        else {
            target->x += (delta_time/time) * (dst.x - src.x);
        }
    }

    if (target->y > dst.y){
        if ((target->y - dst.y) < (delta_time/time) * (src.y - dst.y)){
            target->y = dst.y;
        }
        else {
            target->y -= (delta_time/time) * (src.y - dst.y);
        }
    }
    else if (target->y < dst.y){
        if ((dst.y - target->y) < (delta_time/time) * (dst.y - src.y)){
            target->y = dst.y;
        }
        else {
            target->y += (delta_time/time) * (dst.y - src.y);
        }
    }
}

Event animation_draw_card(Animation *self, float delta_time){
    vec2_translate_in_fixed_time(
        delta_time,
        self->target,
        self->src,
        self->dst,
        0.3f
    );
    if (self->state == ANIMATION_STATE_PLAYING && self->target->x == self->dst.x && self->target->y == self->dst.y){
        self->state = ANIMATION_STATE_COMPLETED;
    }
    return NULL_EVENT;
}

void animate_from_queue(Animation_Context *p_ac, Event_Context *p_ec, float delta_time){
    if (!anim_queue_empty(p_ac->queue) && anim_is_null(*p_ac->playing_blocking_anim)){
        *p_ac->playing_blocking_anim = dequeue_anim(p_ac->queue);
        p_ac->playing_blocking_anim->state = ANIMATION_STATE_PLAYING;
    }
    else if (!anim_is_null(*p_ac->playing_blocking_anim)){
        if (p_ac->playing_blocking_anim->state == ANIMATION_STATE_COMPLETED){
            *p_ac->playing_blocking_anim = NULL_ANIMATION;
            if (anim_queue_empty(p_ac->queue)){
                Event event = common_event_create(EVENT_ANIM_QUEUE_FINISHED);
                enqueue_event(p_ec->queue, event);
            }
        }
        else{
            p_ac->playing_blocking_anim->anim_func(
                p_ac->playing_blocking_anim,
                delta_time
            );
        }
    }
}

void animate(App_State *as){
    
}