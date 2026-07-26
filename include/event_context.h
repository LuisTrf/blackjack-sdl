#pragma once

#include "widget.h"
#include "events.h"

typedef struct EventContext {
    EventQueue *queue;
    Widget **widget_listeners;

} EventContext;

EventContext* event_context_create(void);
void event_context_destroy(EventContext *ec);
void event_context_widget_listeners_notify_all(EventContext *ec, Event event);
void event_context_widget_listener_add(EventContext *ec, Widget *widget);

