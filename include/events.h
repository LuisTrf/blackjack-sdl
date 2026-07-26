#pragma once

/*
SDL_Event docs
*/

#include <SDL3/SDL.h>

typedef enum EventType {
    EVENT_NONE,
    BUTTON_EVENT_RELEASE_DEAL,
    BUTTON_EVENT_RELEASE_HIT
} EventType;

typedef struct Common_Event {
    EventType type;
} Common_Event;

typedef struct Button_Event {
    EventType type;
    struct Button *button_obj;
} Button_Event;

typedef union Event {
    EventType type;
    Common_Event common;
    Button_Event button;
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
