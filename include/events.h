#pragma once

#define MAXIMUM_POLLED_EVENTS 32

typedef enum Event {
    _NONE_EVENT_TYPE
} Event;

typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;

void enqueue_event(Event event);
void initialize_event_queue(void);
void poll_events(void);

