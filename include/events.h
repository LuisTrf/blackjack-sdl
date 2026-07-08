#ifndef EVENTS_H
#define EVENTS_H

#include "game_object.h"
#include "game_objects.h"

#define MAXIMUM_POLLED_EVENTS 32

typedef enum EventType {
    _NONE_EVENT_TYPE,
    END_OF_DRAW_ANIM_EVENT,
    END_OF_BET_ANIM_EVENT,
    END_OF_RETURN_ANIM_EVENT,
    DISABLE_BUTTONS_EVENT,
    RESTORE_BUTTONS_EVENT,
    ANIMATION_QUEUE_FINISHED_EVENT,
    NEW_GAME_EVENT
} EventType;

typedef union EventCargo {
    GameObject *game_object;
    TexturedGameObject *textured_game_object;
} EventCargo;

typedef struct Event {
    EventType type;
    EventCargo cargo;
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

#endif