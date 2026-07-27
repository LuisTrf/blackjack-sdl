#pragma once

/*
SDL_Event docs
*/
#include "vec2.h"
#include <SDL3/SDL.h>

typedef enum EventType {
    EVENT_NONE,
    EVENT_ANIM_QUEUE_FINISHED,
    BUTTON_EVENT_RELEASE_DEAL,
    BUTTON_EVENT_RELEASE_HIT,
    BUTTON_EVENT_RELEASE_STAND,
    GAME_EVENT_NEW_GAME,
    CARD_EVENT_CARD_DRAWN,
    VEC2ANIM_EVENT_CREATE_NEW
} EventType;

typedef struct Common_Event {
    EventType type;
} Common_Event;

typedef struct Button_Event {
    EventType type;
    struct Button *button_obj;
} Button_Event;

typedef struct Card_Event {
    EventType type;
    struct Card *card;
    int dcih;
    int pcih;
} Card_Event;

typedef struct Game_Event {
    EventType type;
    struct Dealer *dealer;
    struct Player *player;
} Game_Event;

typedef struct Vec2Anim_Event {
    EventType type;
    vec2 *tgt;
    vec2 dst;
} Vec2Anim_Event;

typedef union Event {
    EventType type;
    Common_Event common;
    Button_Event button;
    Card_Event card;
    Game_Event game;
    Vec2Anim_Event v2anim;
} Event;

typedef struct EventQueue {
    int size;
    int head;
    int tail;
    Event* arr;
} EventQueue;

static const Event NULL_EVENT = {.type = EVENT_NONE};

Event common_event_create(EventType event_type);
Event button_event_create(EventType event_type, struct Button *button);
Event vec2anim_event_create_with_vec2_dst(EventType event_type, vec2 *tgt, vec2 dst);
Event vec2anim_event_create_with_float_dst_coords(EventType event_type, vec2 *tgt, float dst_x, float dst_y);

EventQueue* event_queue_create(int size);
void event_queue_destroy(EventQueue *p_queue);
bool event_queue_full(EventQueue *queue);
bool event_queue_empty(EventQueue *queue);
void enqueue_event(EventQueue *queue, Event event);
Event dequeue_event(EventQueue *queue);
bool event_is_null(Event event);
/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
