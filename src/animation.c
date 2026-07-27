#include <stdlib.h>
#include <stdio.h>
#include "../include/animation.h"
#include "../include/events.h"
#include "../include/event_context.h"
#include "../include/animation_context.h"

Animation vec2_animation_create(vec2 *target, vec2 dst, Event (*anim_func)(Animation *self, float delta_time)){
    Animation vec2_anim = {
        .type = ANIMATION_TYPE_VEC2,
        .state = ANIMATION_STATE_WAITING,
        .target = target,
        .src = *target,
        .dst = dst,
        .anim_func = anim_func,
    };
    return vec2_anim;
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

AnimationContext* animation_context_create(void){
    AnimationQueue *queue = anim_queue_create(16);
    Animation *p_anim = malloc(sizeof(Animation));
    if (p_anim == NULL){
        abort();
    }
    *p_anim = NULL_ANIMATION;
    AnimationContext ac = {queue, p_anim};
    AnimationContext *p_ac = malloc(sizeof(AnimationContext));
    if (p_ac == NULL){
        abort();
    }
    *p_ac = ac;
    return p_ac;
}

void animation_context_destroy(AnimationContext *p_ac){
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
    if (anim.type == NULL_ANIMATION.type){
        return true;
    }
    else {
        return false;
    }
}

void animation_vec2_translate_in_fixed_time(float delta_time, vec2 *target, vec2 src, vec2 dst, float time){
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
    animation_vec2_translate_in_fixed_time(
        delta_time,
        self->target,
        self->src,
        self->dst,
        0.3f
    );
    if (self->state == ANIMATION_STATE_PLAYING && self->target->x == self->dst.x && self->target->y == self->dst.y){
        printf("completed!\n");
        self->state = ANIMATION_STATE_COMPLETED;
    }
    return NULL_EVENT;
}

void animate_from_queue(AnimationContext *p_ac, EventContext *p_ec, UpdateContext *p_uc){
    if (!anim_queue_empty(p_ac->queue) && anim_is_null(*p_ac->playing_blocking_anim)){
        printf("trying to dequeue!\n");
        *p_ac->playing_blocking_anim = dequeue_anim(p_ac->queue);
        switch (p_ac->playing_blocking_anim->type){
            case ANIMATION_TYPE_VEC2:
                p_ac->playing_blocking_anim->state = ANIMATION_STATE_PLAYING;
            default:
                break;
        }
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
            switch (p_ac->playing_blocking_anim->type){
                case ANIMATION_TYPE_VEC2:
                    p_ac->playing_blocking_anim->anim_func(
                        p_ac->playing_blocking_anim,
                        p_uc->delta_time
                    );
                    break;
                default:
                    break;
            }
        }
    }
}