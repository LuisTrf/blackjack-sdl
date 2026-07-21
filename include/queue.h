#pragma once

#include <stdbool.h>

typedef struct Queue {
    int size;
    int head;
    int tail;
    void** arr;
} Queue;

Queue* queue_create(int size);
void queue_destroy(Queue* queue);
bool queue_full(Queue *queue);
bool queue_empty(Queue *queue);
void enqueue(Queue *queue, void* element);
void* dequeue(Queue *queue);
