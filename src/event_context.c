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
    arrfree(ec->widget_listeners);
    ec->widget_listeners = NULL;
    free(ec);
}

void event_context_widget_listeners_notify_all(EventContext *ec, Event event){
    for (int i = 0; i < arrlen(ec->widget_listeners); i++){
        ec->widget_listeners[i]->notify_func(ec->widget_listeners[i], event);
    }
}

void event_context_widget_listener_add(EventContext *ec, Widget *widget){
    arrput(ec->widget_listeners, widget);
}