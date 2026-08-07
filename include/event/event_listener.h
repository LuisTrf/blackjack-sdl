#pragma once

#include "event_types.h"

typedef struct EventListener {
    void* self;
    void (*notify_func)(void *self, Event event, void* dependencies);
} EventListener;