#pragma once

#include "event_types.h"

typedef struct EventListener {
    void* self;
    Event (*notify_func)(void *self, Event event);
} EventListener;