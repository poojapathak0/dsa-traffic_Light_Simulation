#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include "vehicle.h"

#define MAX_QUEUE_SIZE 100

typedef struct {
    Vehicle* vehicles[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
    pthread_mutex_t mutex;
} VehicleQueue;

void queue_init(VehicleQueue* queue);
bool queue_enqueue(VehicleQueue* queue, Vehicle* vehicle);
Vehicle* queue_dequeue(VehicleQueue* queue);
bool queue_is_empty(VehicleQueue* queue);
void queue_destroy(VehicleQueue* queue);
int queue_size(VehicleQueue* queue);

#endif
