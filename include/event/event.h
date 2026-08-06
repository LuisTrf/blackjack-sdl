#pragma once

#include <SDL3/SDL.h>
#include "event_types.h"
#include "event_listener.h"
#include "../ui/widget.h"

typedef struct EventQueue {
    int size;
    int head;
    int tail;
    Event* arr;
} EventQueue;

typedef struct EventContext {
    EventQueue *queue;
    EventListener* event_listeners;
} EventContext;

Event common_event_create(EventType event_type);
Event animation_event_create(EventType event_type, Rect *target);

EventQueue* event_queue_create(int size);
void event_queue_destroy(EventQueue *queue);
bool event_queue_full(EventQueue *queue);
bool event_queue_empty(EventQueue *queue);
void event_enqueue(EventQueue *queue, Event event);
Event event_dequeue(EventQueue *queue);
bool event_is_null(Event event);

EventListener event_listener_create(void *self, Event (*notify_func)(void *self, Event event, Cargo *cargo));

EventContext* event_context_create(void);
void event_context_destroy(EventContext *event_ctx);
void event_ctx_listeners_notify_all(EventContext *event_ctx, Event event, Cargo *cargo);
void event_ctx_listener_register(EventContext *event_ctx, EventListener event_listener);
/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
