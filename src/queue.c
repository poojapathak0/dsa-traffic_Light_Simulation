#include "queue.h"
#include <stdlib.h>

void queue_init(VehicleQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    pthread_mutex_init(&queue->mutex, NULL);
}

bool queue_enqueue(VehicleQueue* queue, Vehicle* vehicle) {
    pthread_mutex_lock(&queue->mutex);
    
    if (queue->size >= MAX_QUEUE_SIZE) {
        pthread_mutex_unlock(&queue->mutex);
        return false;
    }

    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    queue->vehicles[queue->rear] = vehicle;
    queue->size++;

    pthread_mutex_unlock(&queue->mutex);
    return true;
}

Vehicle* queue_dequeue(VehicleQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    
    if (queue->size == 0) {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }

    Vehicle* vehicle = queue->vehicles[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    queue->size--;

    pthread_mutex_unlock(&queue->mutex);
    return vehicle;
}

bool queue_is_empty(VehicleQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    bool empty = (queue->size == 0);
    pthread_mutex_unlock(&queue->mutex);
    return empty;
}

int queue_size(VehicleQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    int size = queue->size;
    pthread_mutex_unlock(&queue->mutex);
    return size;
}

void queue_destroy(VehicleQueue* queue) {
    pthread_mutex_destroy(&queue->mutex);
}
