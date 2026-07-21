#pragma once

/*
SDL_Event docs
*/

#include <SDL3/SDL.h>

typedef enum App_EventType {
    APP_EVENT_TYPE_SDL,
    APP_EVENT_TYPE_COMMON
} App_EventType;

typedef enum Common_EventType {
    EVENT_RELEASE_DEAL,
    EVENT_RELEASE_HIT
} Common_EventType;

typedef struct Common_Event {
    Common_EventType type;
} Common_Event;

typedef struct App_SDL_Event {
    App_EventType type;
    SDL_Event event;
} App_SDL_Event;

typedef struct App_Common_Event {
    App_EventType type;
    Common_Event event;
} App_CommonEvent;

typedef union App_Event {
    App_EventType type;
    App_SDL_Event sdl;
    App_CommonEvent common;
} App_Event;

App_Event event_app_common_event_create(Common_EventType event_type);
/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
