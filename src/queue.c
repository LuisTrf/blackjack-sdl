#include <stdlib.h>
#include <stdio.h>
#include "../include/queue.h"

Queue* queue_create(int size){
    void **arr = calloc(size, sizeof(void*));
    if (arr == NULL){
        abort();
    }
    Queue queue = {size, 0, 0, arr};
    Queue* p_queue = malloc(sizeof(Queue));
    if (p_queue == NULL){
        abort();
    }

    *p_queue = queue;
    return p_queue;
}

void queue_destroy(Queue* queue){
    free(queue->arr);
    queue->arr = NULL;
    free(queue);
}

bool queue_full(Queue *queue){
    if (
        queue->head==queue->tail+1 
        || (queue->head==0 && queue->tail==queue->size-1)
    ){
        return true;
    }
    else {
        return false;
    }
}

bool queue_empty(Queue *queue){
    if (queue->head==queue->tail) {
        return true;
    }
    else {
        return false;
    }
}

void enqueue(Queue *queue, void* element){
    if (queue_full(queue)){
        fprintf(stderr, "EVENT QUEUE OVERFLOW!");
        return;
    }
    else{
        queue->arr[queue->tail] = element;
        if (queue->tail==queue->size-1){
            queue->tail=0;
        }
        else {
            queue->tail++;
        }
    }
}

void* dequeue(Queue *queue){
    if (queue_empty(queue)){
        fprintf(stderr, "EVENT QUEUE UNDERFLOW!");
        return NULL;
    }
    else {
        void* element = queue->arr[queue->head];
        if (queue->head==queue->size-1){
            queue->head=0;
        }
        else {
            queue->head++;
        }
        return element;
    }
}