#pragma once

#include "event.h"

typedef struct EventQueue {
    int size;
    int head;
    int tail;
    Event* arr;
} EventQueue;

typedef struct Event_Context {
    EventQueue *queue;
    Widget **event_widget_listeners;
} EventContext;