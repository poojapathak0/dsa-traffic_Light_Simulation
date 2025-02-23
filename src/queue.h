#ifndef QUEUE_H
#define QUEUE_H

#include "vehicle.h"

typedef struct Node {
    Vehicle* vehicle;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} VehicleQueue;

void queue_init(VehicleQueue* queue);
void queue_enqueue(VehicleQueue* queue, Vehicle* vehicle);
Vehicle* queue_dequeue(VehicleQueue* queue);
bool queue_is_empty(VehicleQueue* queue);
void queue_destroy(VehicleQueue* queue);

#endif // QUEUE_H