#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/update.h"
#include "../include/animate.h"
#include "../include/animation_context.h"
#include "../include/events.h"

static const Animation NULL_ANIMATION = {NULL, 0, 0, 0, 0, false, false, NULL};
static AnimationQueue anim_queue;
static Animation *anim_pool[ANIMATION_POOL_SIZE];

bool is_null_animation(Animation *anim){
    return (
        anim->target==NULL
        && anim->src_x==0
        && anim->src_y==0
        && anim->dst_x==0
        && anim->dst_y==0
        && anim->playing==false
        && anim->completed==false
        && anim->anim_func==NULL
    );
}

void initialize_anim_queue(void){
    for (int i=0; i<ANIMATION_QUEUE_SIZE; i++){
        Animation anim = NULL_ANIMATION;
        Animation *p = malloc(sizeof(Animation));
        if (p==NULL){
            abort();
        }
        *p = anim;
        anim_queue.arr[i]=p;
    }
    anim_queue.state=QUEUE_IDLE;
    anim_queue.playing_anim=NULL;
    anim_queue.head=0;
    anim_queue.tail=0;
    anim_queue.size=ANIMATION_QUEUE_SIZE;
    anim_queue.blocking=false;
}

void initialize_anim_pool(void){
    for (int i=0; i<ANIMATION_POOL_SIZE; i++){
        Animation anim = NULL_ANIMATION;
        Animation *p = malloc(sizeof(Animation));
        if (p==NULL){
            abort();
        }
        *p = anim;
        anim_pool[i]=p;
    }
}

void free_allocated_anim_queue_memory(void){
    for (int i=0; i<ANIMATION_QUEUE_SIZE; i++){
        free(anim_queue.arr[i]);
        anim_queue.arr[i]=NULL;
    }
}

void free_allocated_anim_pool_memory(void){
    for (int i=0; i<ANIMATION_QUEUE_SIZE; i++){
        free(anim_queue.arr[i]);
        anim_queue.arr[i]=NULL;
    }
}

bool anim_queue_full(void){
    if (
        anim_queue.head==anim_queue.tail+1 
        || (anim_queue.head==0 && anim_queue.tail==anim_queue.size-1)
    ){
        return true;
    }
    else {
        return false;
    }
}

bool anim_queue_empty(void){
    if (anim_queue.head==anim_queue.tail) {
        return true;
    }
    else {
        return false;
    }
}

void enqueue_anim(Animation animation){
    if (anim_queue_full()){
        fprintf(stderr, "Animation queue overflow.");
        return;
    }
    else{
        *anim_queue.arr[anim_queue.tail] = animation;
        if (anim_queue.tail==anim_queue.size-1){
            anim_queue.tail=0;
        }
        else {
            anim_queue.tail++;
        }
    }
}

Animation* dequeue_anim(void){
    if (anim_queue_empty()){
        fprintf(stderr, "Animation queue underflow.");
        return NULL;
    }
    else {
        Animation *dequeued_anim_pointer = anim_queue.arr[anim_queue.head];
        if (anim_queue.head==anim_queue.size-1){
            anim_queue.head=0;
        }
        else {
            anim_queue.head++;
        }
        return dequeued_anim_pointer;
    }
}

int get_avaible_anim_pool_index(void){
    for (int i=0; i<ANIMATION_POOL_SIZE; i++){
        if (is_null_animation(anim_pool[i])) {return i;}
    }
    return -1;
}

void add_anim(Animation anim){
    int i = get_avaible_anim_pool_index();
    if (i==-1) {
        fprintf(stderr, "ANIMATION POOL IS FULL!\n");
        return;
    }
    *anim_pool[i]=anim;
}

int get_anim_index(Animation *anim){
    for (int i=0; i<ANIMATION_POOL_SIZE; i++){
        if (
            &anim_pool[i]->target==&anim->target
            && anim_pool[i]->src_x==anim->src_x
            && anim_pool[i]->src_y==anim->src_y
            && anim_pool[i]->dst_x==anim->dst_x
            && anim_pool[i]->dst_y==anim->dst_y
            && anim_pool[i]->playing==anim->playing
            && anim_pool[i]->completed==anim->completed
            && anim_pool[i]->anim_func==anim->anim_func
        ) 
        {return i;}
    }
    return -1;
}

void remove_anim(Animation *anim){
    int i = get_anim_index(anim);
    if (i==-1) {
        fprintf(stderr, "ANIMATION DOES NOT EXIST IN POOL!\n");
        return;
    }
    Animation null_anim = NULL_ANIMATION;
    *anim_pool[i]=null_anim;
}

void translate_in_fixed_time(vec2 *target, float dst_x, float dst_y, float src_x, float src_y, float time){
    if (target->x > dst_x){
        if ((target->x - dst_x) < (get_delta_time()/time) * (src_x - dst_x)){
            target->x=dst_x;
        }
        else {
            target->x -= (get_delta_time()/time) * (src_x - dst_x);
        }
    }
    else if (target->x < dst_x){
        if ((dst_x - target->x) < (get_delta_time()/time) * (dst_x - src_x)){
            target->x=dst_x;
        }
        else {
            target->x += (get_delta_time()/time) * (dst_x - src_x);
        }
    }

    if (target->y > dst_y){
        if ((target->y - dst_y) < (get_delta_time()/time) * (src_y - dst_y)){
            target->y=dst_y;
        }
        else {
            target->y -= (get_delta_time()/time) * (src_y - dst_y);
        }
    }
    else if (target->y < dst_y){
        if ((dst_y - target->y) < (get_delta_time()/time) * (dst_y - src_y)){
            target->y=dst_y;
        }
        else {
            target->y += (get_delta_time()/time) * (dst_y - src_y);
        }
    }
}

void card_draw_anim(Animation* self){
    translate_in_fixed_time(
        self->target,
        self->dst_x,
        self->dst_y,
        self->src_x,
        self->src_y,
        0.3f
    );
    if (self->playing && self->target->x == self->dst_x && self->target->y == self->dst_y){
        self->completed=true;
        self->playing=false;
        Event event = {.type=END_OF_DRAW_ANIM_EVENT, .cargo.game_object=self->target};
        enqueue_event(event);
    }
}

void bet_anim(Animation* self){
    translate_in_fixed_time(
        self->target,
        self->dst_x,
        self->dst_y,
        self->src_x,
        self->src_y,
        0.15f
    );
    if (self->playing && self->target->x == self->dst_x && self->target->y == self->dst_y){
        self->completed=true;
        self->playing=false;
        Event event = {.type=END_OF_BET_ANIM_EVENT, .cargo.game_object=self->target};
        enqueue_event(event);
    }
}

void return_anim(Animation* self){
    translate_in_fixed_time(
        self->target,
        self->dst_x,
        self->dst_y,
        self->src_x,
        self->src_y,
        0.15f
    );
    if (self->playing && self->target->x == self->dst_x && self->target->y == self->dst_y){
        self->completed=true;
        self->playing=false;
        Event event = {.type=END_OF_RETURN_ANIM_EVENT, .cargo.game_object=self->target};
        enqueue_event(event);
    }
}

void move_030_anim(Animation *self){
    translate_in_fixed_time(
        self->target,
        self->dst_x,
        self->dst_y,
        self->src_x,
        self->src_y,
        0.3f
    );
    if (self->playing && self->target->x == self->dst_x && self->target->y == self->dst_y){
        self->completed=true;
        self->playing=false;
    }
}

Animation create_anim(GameObject *obj, float src_x, float src_y, float dst_x, float dst_y, void (*anim_func)(Animation *self)){
    Animation anim = NULL_ANIMATION;
    obj->x=src_x;
    obj->y=src_y;
    anim.target=obj;
    anim.src_x=obj->x;
    anim.src_y=obj->y;
    anim.dst_x=dst_x;
    anim.dst_y=dst_y;
    anim.anim_func=anim_func;
    return anim;
}

bool is_anim_queue_blocking(void){
    return anim_queue.blocking;
}

void block(void){
    if (!anim_queue_empty() && !anim_queue.blocking){
        anim_queue.blocking=true;
        Event event = {.type=DISABLE_BUTTONS_EVENT, NULL};
        enqueue_event(event);
    }
    else if (anim_queue_empty() && anim_queue.playing_anim==NULL && anim_queue.blocking){
        anim_queue.blocking=false;
        Event event = {.type=RESTORE_BUTTONS_EVENT, NULL};
        enqueue_event(event);
    }
}

ANIMATION_QUEUE_STATE get_anim_queue_state(void){
    return anim_queue.state;
}

void animate_from_queue(void){
    if (!anim_queue_empty() && anim_queue.playing_anim==NULL){
        anim_queue.state=WORKING;
        anim_queue.playing_anim=dequeue_anim();
        anim_queue.playing_anim->playing=true;
        anim_queue.playing_anim->target->visible=true;
    }
    else if (anim_queue.playing_anim!=NULL){
        if (anim_queue.playing_anim->completed){
            if (anim_queue_empty()){
                anim_queue.state=QUEUE_IDLE;
                Event event = {.type=ANIMATION_QUEUE_FINISHED_EVENT, NULL};
                enqueue_event(event);
            }
            anim_queue.playing_anim=NULL;
        }
        else{
            anim_queue.playing_anim->anim_func(anim_queue.playing_anim);
        }
    }
}

void animate_from_pool(void){
    for (int i=0; i<ANIMATION_POOL_SIZE; i++){
        Animation *anim = anim_pool[i];
        if (!is_null_animation(anim)){
            if (!anim->playing){
                if (anim->completed){
                    remove_anim(anim);
                }
                else {
                    anim->playing=true;
                }
            }
            else {
                anim->anim_func(anim);
            }
        }
    }
}

void animate(void){
    block();
    animate_from_queue();
    animate_from_pool();
}