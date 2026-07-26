#pragma once

#include "widget.h"
#include "events.h"

typedef struct EventContext {
    EventQueue *queue;
    Widget **event_widget_listeners;
} EventContext;

EventContext* event_context_create(void);
void event_context_destroy(EventContext *ec);
void ec_widget_listeners_notify_all(EventContext *ec, Event event);
void ec_widget_listener_register(EventContext *ec, Widget *widget);

