#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "vehicle.h"

typedef struct Node {
    Vehicle* vehicle;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
} PriorityQueue;

void priority_queue_init(PriorityQueue* queue);
void priority_queue_enqueue(PriorityQueue* queue, Vehicle* vehicle);
Vehicle* priority_queue_dequeue(PriorityQueue* queue);
bool priority_queue_is_empty(PriorityQueue* queue);
void priority_queue_destroy(PriorityQueue* queue);

#endif // PRIORITY_QUEUE_H