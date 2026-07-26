#pragma once

/*
SDL_Event docs
*/

#include <SDL3/SDL.h>

typedef enum EventType {
    EVENT_TYPE_NONE,
    EVENT_TYPE_SDL,
    EVENT_TYPE_APP
} EventType;

typedef enum App_EventType {
    BUTTON_EVENT_RELEASE_DEAL,
    BUTTON_EVENT_RELEASE_HIT
} App_EventType;

typedef struct Common_Event {
    App_EventType type;
} Common_Event;

typedef struct Button_Event {
    App_EventType type;
    struct Button *button_obj;
} Button_Event;

typedef union App_Event {
    App_EventType type;
    Common_Event common;
    Button_Event button;
} App_Event;

typedef struct EventTyped_SDL_Event {
    EventType type;
    SDL_Event event;
} EventTyped_SDL_Event;

typedef struct EventTyped_App_Event {
    EventType type;
    App_Event event;
} EventTyped_App_Event;

typedef union Event {
    EventType type;
    EventTyped_SDL_Event sdl;
    EventTyped_App_Event app;
} Event;

typedef struct EventQueue {
    int size;
    int head;
    int tail;
    Event* arr;
} EventQueue;

static const Event NULL_EVENT = {.type = EVENT_TYPE_NONE};

Event eventtyped_app_event_create(App_EventType event_type);
EventQueue* event_queue_create(int size);
void event_queue_destroy(EventQueue *p_queue);
bool event_queue_full(EventQueue *queue);
bool event_queue_empty(EventQueue *queue);
void enqueue_event(EventQueue *queue, Event event);
Event dequeue_event(EventQueue *queue);
/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
