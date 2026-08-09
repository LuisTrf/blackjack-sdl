#pragma once

#include <SDL3/SDL.h>
#include "event_types.h"
#include "event_listener.h"
#include "../ui/widget.h"

typedef struct EventQueue {
    int size;
    int count;
    int head;
    int tail;
    Event* arr;
} EventQueue;

EventQueue* event_queue_create(int size);
void event_queue_destroy(EventQueue *queue);
bool event_queue_full(EventQueue *queue);
bool event_queue_empty(EventQueue *queue);
void event_enqueue(EventQueue *queue, Event event);
Event event_dequeue(EventQueue *queue);
bool event_is_null(Event event);

void event_listeners_notify_all(EventListener **event_listeners, Event event, void *dependencies);
void event_listener_register(EventListener **event_listeners, EventListener event_listener);
/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
