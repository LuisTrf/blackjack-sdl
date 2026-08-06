#pragma once

#include "event_types.h"
#include "../cargo.h"

typedef struct EventListener {
    void* self;
    Event (*notify_func)(void *self, Event event, Cargo* cargo);
} EventListener;