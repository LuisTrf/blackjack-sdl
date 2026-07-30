#pragma once

/*
SDL_Event docs
*/
#include <SDL3/SDL.h>
#include "event_types.h"
#include "../ui/widget.h"

typedef struct EventQueue EventQueue;
typedef struct Event_Context Event_Context;

Event common_event_create(EventType event_type);

EventQueue* event_queue_create(int size);
void event_queue_destroy(EventQueue *p_queue);
bool event_queue_full(EventQueue *queue);
bool event_queue_empty(EventQueue *queue);
void enqueue_event(EventQueue *queue, Event event);
Event dequeue_event(EventQueue *queue);
bool event_is_null(Event event);

Event_Context* event_context_create(void);
void event_context_destroy(Event_Context *ec);
void ec_widget_listeners_notify_all(Event_Context *ec, Event event);
void ec_widget_listener_register(Event_Context *ec, Widget *widget);

/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
