#include <stdlib.h>
#include "../include/stb_ds.h"
#include "../include/event_context.h"

EventContext* event_context_create(void){
    EventQueue *queue = event_queue_create(32);
    EventContext ec = {queue, NULL};
    EventContext *p_ec = malloc(sizeof(EventContext));
    if (p_ec == NULL){
        abort();
    }
    *p_ec = ec;
    return p_ec;
}

void event_context_destroy(EventContext *ec){
    event_queue_destroy(ec->queue);
    ec->queue = NULL;
    arrfree(ec->event_widget_listeners);
    ec->event_widget_listeners = NULL;
    free(ec);
}

void ec_widget_listeners_notify_all(EventContext *ec, Event event){
    for (int i = 0; i < arrlen(ec->event_widget_listeners); i++){
        Event e = ec->event_widget_listeners[i]->notify_func(ec->event_widget_listeners[i], event);
        if (!event_is_null(e)){
            enqueue_event(ec->queue, e);
        }
    }
}

void ec_widget_listener_register(EventContext *ec, Widget *widget){
    arrput(ec->event_widget_listeners, widget);
}