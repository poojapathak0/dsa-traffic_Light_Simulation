#include "traffic_system.h"
#include <stdlib.h>

void initializeTrafficLight(TrafficLight* light, int greenDuration, int yellowDuration, int redDuration) {
    light->state = TRAFFIC_LIGHT_RED;
    light->lastChangeTime = GetTickCount();
    light->greenDuration = greenDuration;
    light->yellowDuration = yellowDuration;
    light->redDuration = redDuration;
}

void updateTrafficLight(TrafficLight* light) {
    DWORD currentTime = GetTickCount();
    DWORD elapsedTime = currentTime - light->lastChangeTime;

    switch (light->state) {
        case TRAFFIC_LIGHT_GREEN:
            if (elapsedTime >= light->greenDuration) {
                light->state = TRAFFIC_LIGHT_YELLOW;
                light->lastChangeTime = currentTime;
            }
            break;
        case TRAFFIC_LIGHT_YELLOW:
            if (elapsedTime >= light->yellowDuration) {
                light->state = TRAFFIC_LIGHT_RED;
                light->lastChangeTime = currentTime;
            }
            break;
        case TRAFFIC_LIGHT_RED:
            if (elapsedTime >= light->redDuration) {
                light->state = TRAFFIC_LIGHT_GREEN;
                light->lastChangeTime = currentTime;
            }
            break;
    }
}

void enqueueVehicle(VehicleQueue* queue, Vehicle vehicle) {
    VehicleNode* newNode = (VehicleNode*)malloc(sizeof(VehicleNode));
    newNode->vehicle = vehicle;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->count++;
}

Vehicle dequeueVehicle(VehicleQueue* queue) {
    if (queue->front == NULL) {
        Vehicle emptyVehicle = {0};
        return emptyVehicle;
    }

    VehicleNode* temp = queue->front;
    Vehicle vehicle = temp->vehicle;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->count--;
    return vehicle;
}

int isQueueEmpty(VehicleQueue* queue) {
    return queue->count == 0;
}