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
    EVENT_EVENT
} Common_EventType;

typedef struct App_SDL_Event {
    App_EventType type;
    SDL_Event sdl;
} App_SDL_Event;

typedef struct Common_Event {
    Common_EventType type;
} Common_Event;

typedef struct App_Common_Event {
    App_EventType type;
    Common_Event common;
} App_CommonEvent;

typedef union App_Event {
    App_EventType type;
    App_SDL_Event sdl;
    App_CommonEvent common;
} App_Event;


/*
#define MAXIMUM_POLLED_EVENTS 32
typedef struct EventQueue {
    Event arr[MAXIMUM_POLLED_EVENTS];
    unsigned char head;
    unsigned char tail;
    unsigned char size;
} EventQueue;
*/
