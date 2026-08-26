#include <stdlib.h>
#include "../../include/update/animation.h"
#include "../../include/ui/spritebox.h"

AnimationQueue* anim_queue_create(int size){
    Animation* arr = calloc(size, sizeof(Animation));
    if (arr == NULL){
        abort();
    }
    Animation* p_playing_blocking_anim = malloc(sizeof(Animation));
    if (p_playing_blocking_anim == NULL){
        abort();
    }
    *p_playing_blocking_anim = NULL_ANIMATION;
    AnimationQueue queue = {size, 0, .0, 0, false, p_playing_blocking_anim, arr};
    AnimationQueue *p_queue = malloc(sizeof(queue));
    if (p_queue == NULL){
        abort();
    }
    *p_queue = queue;
    return p_queue;
}

void anim_queue_destroy(AnimationQueue *p_queue){
    free(p_queue->playing_blocking_anim);
    free(p_queue->arr);
    p_queue->arr = NULL;
    free(p_queue);
}

bool anim_queue_full(AnimationQueue *queue){
    return (queue->count == queue->size);
}

bool anim_queue_empty(AnimationQueue *queue){
    return (queue->count == 0);
}

void anim_enqueue(AnimationQueue *queue, Animation anim){
    if (anim_queue_full(queue)){
        return;
    }
    queue->arr[queue->tail] = anim;
    queue->tail = (queue->tail + 1) % queue->size;
    ++queue->count;
}

Animation anim_dequeue(AnimationQueue *queue){
    if (anim_queue_empty(queue)){
        return NULL_ANIMATION;
    }
    Animation anim = queue->arr[queue->head];
    queue->head = (queue->head + 1) % queue->size;
    --queue->count;
    return anim;
}

AnimationPool* anim_pool_create(int size){
    AnimationPool *anim_pool = malloc(sizeof(AnimationPool));
    if (anim_pool == NULL){
        abort();
    }
    for (int i = 0; i < size; i++){
        anim_pool->arr[i] = NULL_ANIMATION;
        anim_pool->free_list[i] = size - 1 - i;
    }
    anim_pool->size = size;
    anim_pool->free_count = size;
    return anim_pool;
}

void anim_pool_destroy(AnimationPool *anim_pool){
    free(anim_pool);
}

bool free_list_empty(int free_count){
    return (free_count == 0);
}

bool free_list_full(int free_count, int size){
    return (free_count == size);
}

int anim_add(AnimationPool *anim_pool, Animation anim){
    if (free_list_empty(anim_pool->free_count)) {return -1;}
    int free_index = anim_pool->free_list[--anim_pool->free_count];
    anim_pool->arr[free_index] = anim;
    return free_index;
}

Animation anim_remove(AnimationPool *anim_pool, int index){
    if (free_list_full(anim_pool->free_count, anim_pool->size)) {return NULL_ANIMATION;}
    Animation anim = anim_pool->arr[index];
    anim_pool->arr[index] = NULL_ANIMATION;
    anim_pool->free_list[anim_pool->free_count++] = index;
    return anim;
}

bool anim_is_null(Animation anim){
    return (anim.type == _ANIMATION_TYPE_NONE);
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
        &(self->target->pos),
        self->vec2_anim.src,
        self->vec2_anim.dst,
        0.3f
    );
    if (self->state == ANIMATION_STATE_PLAYING && self->target->pos.x == self->vec2_anim.dst.x && self->target->pos.y == self->vec2_anim.dst.y){
        self->state = ANIMATION_STATE_COMPLETED;
        return (Event){.anim={.type=ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED, .target=self->target}};
    }
    return NULL_EVENT;
}

Event animation_cheque_move(Animation *self, float delta_time){
    vec2_translate_in_fixed_time(
        delta_time,
        &(self->target->pos),
        self->vec2_anim.src,
        self->vec2_anim.dst,
        0.15f
    );
    if (self->state == ANIMATION_STATE_PLAYING && self->target->pos.x == self->vec2_anim.dst.x && self->target->pos.y == self->vec2_anim.dst.y){
        self->state = ANIMATION_STATE_COMPLETED;
        return (Event){.anim={.type=ANIMATION_EVENT_ANIMATION_CHEQUE_COMPLETED, .target=self->target}};
    }
    return NULL_EVENT;
}

Event animation_betting_elements_move(Animation *self, float delta_time){
    vec2_translate_in_fixed_time(
        delta_time,
        &(self->target->pos),
        self->vec2_anim.src,
        self->vec2_anim.dst,
        0.23f
    );
    if (self->state == ANIMATION_STATE_PLAYING && self->target->pos.x == self->vec2_anim.dst.x && self->target->pos.y == self->vec2_anim.dst.y){
        self->state = ANIMATION_STATE_COMPLETED;
    }
    return NULL_EVENT;
}

#include <stdio.h>

Event animation_arrow(Animation *self, float delta_time){
    SpriteBox *arrow = (SpriteBox *)self->target;
    self->sprite_anim.ttnf -= delta_time;
    if (self->sprite_anim.ttnf < 0){
        if (self->sprite_anim.frame_idx == 6){
            self->sprite_anim.frame_idx = 0;
            arrow->spritesheet_x = 0;
        }
        else {
            self->sprite_anim.frame_idx++;
            arrow->spritesheet_x += self->sprite_anim.sheet_step_x;
        }
        self->sprite_anim.ttnf = (1.f / self->sprite_anim.fps);
    }
    return NULL_EVENT;
}

void block(AnimationQueue *anim_queue, EventQueue *event_queue){
    if ((!anim_queue_empty(anim_queue) || !anim_is_null(*anim_queue->playing_blocking_anim)) && !anim_queue->queue_is_blocking){
        anim_queue->queue_is_blocking = true;
        event_enqueue(event_queue, (Event){.anim={.type=ANIMATION_EVENT_QUEUE_BLOCKING, .target=NULL}});
    }
}

void unblock(AnimationQueue *anim_queue, EventQueue *event_queue){
    if (anim_queue_empty(anim_queue) && anim_is_null(*anim_queue->playing_blocking_anim) && anim_queue->queue_is_blocking){
        anim_queue->queue_is_blocking = false;
        event_enqueue(event_queue, (Event){.anim={.type=ANIMATION_EVENT_QUEUE_NONBLOCKING, .target=NULL}});
    }
}

void animate_from_queue(AnimationQueue *anim_queue, EventQueue *event_queue, float delta_time){
    if (!anim_queue_empty(anim_queue) && anim_is_null(*anim_queue->playing_blocking_anim)){
        *anim_queue->playing_blocking_anim = anim_dequeue(anim_queue);
        anim_queue->playing_blocking_anim->state = ANIMATION_STATE_PLAYING;
        block(anim_queue, event_queue);
    }
    else if (!anim_is_null(*anim_queue->playing_blocking_anim)){
        if (anim_queue->playing_blocking_anim->state == ANIMATION_STATE_COMPLETED){
            *anim_queue->playing_blocking_anim = NULL_ANIMATION;
            unblock(anim_queue, event_queue);
        }
        else{
            Event event = anim_queue->playing_blocking_anim->anim_func(
                anim_queue->playing_blocking_anim,
                delta_time
            );
            if (!event_is_null(event)) {event_enqueue(event_queue, event);}
        }
    }
}

void animate_from_pool(AnimationPool *anim_pool, EventQueue *event_queue, float delta_time){
    for (int i = 0; i < anim_pool->size; i++){
        if (anim_is_null(anim_pool->arr[i])) {continue;}
        switch (anim_pool->arr[i].state){
            case ANIMATION_STATE_WAITING:
                anim_pool->arr[i].state = ANIMATION_STATE_PLAYING;
                break;
            case ANIMATION_STATE_PLAYING: {
                Event event = anim_pool->arr[i].anim_func(&anim_pool->arr[i], delta_time);
                if (!event_is_null(event)) {event_enqueue(event_queue, event);}
                break;
            }
            case ANIMATION_STATE_COMPLETED:
                anim_remove(anim_pool, i);
                break;
            default:
                break;
        }
    }
}

void animate(AnimationQueue *anim_queue, AnimationPool *anim_pool, EventQueue *event_queue, float delta_time){
    animate_from_queue(
        anim_queue, 
        event_queue, 
        delta_time
    );
    animate_from_pool(
        anim_pool, 
        event_queue,
        delta_time
    );
}