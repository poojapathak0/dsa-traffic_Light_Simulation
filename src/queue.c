#include "queue.h"
#include <stdlib.h>

void queue_init(VehicleQueue* queue) {
    queue->front = queue->rear = NULL;
}

void queue_enqueue(VehicleQueue* queue, Vehicle* vehicle) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode) {
        newNode->vehicle = vehicle;
        newNode->next = NULL;
        if (queue->rear) {
            queue->rear->next = newNode;
        } else {
            queue->front = newNode;
        }
        queue->rear = newNode;
    }
}

Vehicle* queue_dequeue(VehicleQueue* queue) {
    if (queue->front) {
        Node* temp = queue->front;
        Vehicle* vehicle = temp->vehicle;
        queue->front = queue->front->next;
        if (!queue->front) {
            queue->rear = NULL;
        }
        free(temp);
        return vehicle;
    }
    return NULL;
}

bool queue_is_empty(VehicleQueue* queue) {
    return queue->front == NULL;
}

void queue_destroy(VehicleQueue* queue) {
    while (!queue_is_empty(queue)) {
        Vehicle* vehicle = queue_dequeue(queue);
        vehicle_destroy(vehicle);
    }
}