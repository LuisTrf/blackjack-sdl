#ifndef ANIMATE_H
#define ANIMATE_H

#include <stdbool.h>
#include "game.h"
#include "game_object.h"
#include "game_objects.h"

/*
Queue is blocking; pool is non-blocking.
*/

#define ANIMATION_QUEUE_SIZE 8
#define ANIMATION_POOL_SIZE 16

typedef enum ANIMATION_QUEUE_STATE {
    QUEUE_IDLE,
    WORKING
} ANIMATION_QUEUE_STATE;

typedef struct Animation Animation;
struct Animation{
    GameObject *target;
    float src_x;
    float src_y;
    float dst_x;
    float dst_y;
    bool playing;
    bool completed;
    void (*anim_func)(Animation *self);
};

typedef struct {
    Animation *arr[ANIMATION_QUEUE_SIZE];
    ANIMATION_QUEUE_STATE state;
    Animation *playing_anim;
    unsigned char head;
    unsigned char tail;
    unsigned char size;
    bool blocking;
} AnimationQueue;

void initialize_anim_queue(void);
void initialize_anim_pool(void);
void free_allocated_anim_queue_memory(void);
void free_allocated_anim_pool_memory(void);
void enqueue_anim(Animation card_animation);
void card_draw_anim(Animation *self);
bool is_anim_queue_blocking(void);
ANIMATION_QUEUE_STATE get_anim_queue_state(void);

Animation create_anim(GameObject *obj, float src_x, float src_y, float dst_x, float dst_y, void (*anim_func)(Animation *self));
void add_anim(Animation anim);
void bet_anim(Animation *self);
void return_anim(Animation *self);
void move_030_anim(Animation *self);

void animate(void);

#endif