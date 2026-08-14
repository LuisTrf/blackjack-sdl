#include <SDL3/SDL.h>
#include <stdlib.h>
#include "../../include/stb_ds.h"
#include "../../include/event/event.h"

EventQueue* event_queue_create(int size){
    Event* arr = calloc(size, sizeof(Event));
    if (arr == NULL){
        abort();
    }
    EventQueue queue = {.size=size, .count=0, .head=0, .tail=0, .arr=arr};
    EventQueue *p_queue = malloc(sizeof(queue));
    if (p_queue == NULL){
        abort();
    }
    *p_queue = queue;
    return p_queue;
}

void event_queue_destroy(EventQueue *queue){
    free(queue->arr);
    queue->arr = NULL;
    free(queue);
}

bool event_queue_full(EventQueue *queue){
    return (queue->count == queue->size);
}

bool event_queue_empty(EventQueue *queue){
    return (queue->count == 0);
}

void event_enqueue(EventQueue *queue, Event event){
    if (event_queue_full(queue)){
        return;
    }
    queue->arr[queue->tail] = event;
    queue->tail = (queue->tail + 1) % queue->size;
    ++queue->count;
}

Event event_dequeue(EventQueue *queue){
    if (event_queue_empty(queue)){
        return NULL_EVENT;
    }
    Event e = queue->arr[queue->head];
    queue->head = (queue->head + 1) % queue->size;
    --queue->count;
    return e;
}

bool event_is_null(Event event){
    return (event.type == NULL_EVENT.type);
}

void event_listeners_notify_all(EventListener **event_listeners, Event event, void *dependencies){
    for (int i = 0; i < arrlen(*event_listeners); i++){
        (*event_listeners)[i].notify_func((void *)(*event_listeners)[i].self, event, dependencies);
    }
}

void event_listener_register(EventListener **event_listeners, EventListener event_listener){
    arrput(*event_listeners, event_listener);
}