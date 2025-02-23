#include "priority_queue.h"
#include <stdlib.h>

void priority_queue_init(PriorityQueue* queue) {
    queue->front = NULL;
}

void priority_queue_enqueue(PriorityQueue* queue, Vehicle* vehicle) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode) {
        newNode->vehicle = vehicle;
        newNode->next = NULL;

        if (!queue->front || vehicle->type > queue->front->vehicle->type) {
            newNode->next = queue->front;
            queue->front = newNode;
        } else {
            Node* current = queue->front;
            while (current->next && current->next->vehicle->type >= vehicle->type) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

Vehicle* priority_queue_dequeue(PriorityQueue* queue) {
    if (queue->front) {
        Node* temp = queue->front;
        Vehicle* vehicle = temp->vehicle;
        queue->front = queue->front->next;
        free(temp);
        return vehicle;
    }
    return NULL;
}

bool priority_queue_is_empty(PriorityQueue* queue) {
    return queue->front == NULL;
}

void priority_queue_destroy(PriorityQueue* queue) {
    while (!priority_queue_is_empty(queue)) {
        Vehicle* vehicle = priority_queue_dequeue(queue);
        vehicle_destroy(vehicle);
    }
}