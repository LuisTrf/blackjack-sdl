#pragma once

/*
SDL_Event docs
*/

#include <SDL3/SDL.h>

typedef enum EventType {
    EVENT_TYPE_SDL,
    EVENT_TYPE_APP
} EventType;

typedef enum App_EventType {
    BUTTON_EVENT_RELEASE_DEAL,
    BUTTON_EVENT_RELEASE_HIT
} App_EventType;

typedef struct App_Event {
    App_EventType type;
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

Event event_app_event_create(App_EventType event_type);
/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
