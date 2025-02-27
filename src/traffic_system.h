#ifndef TRAFFIC_SYSTEM_H
#define TRAFFIC_SYSTEM_H

#include <windows.h>
#include "vehicle.h"

typedef enum {
    TRAFFIC_LIGHT_RED,
    TRAFFIC_LIGHT_GREEN,
    TRAFFIC_LIGHT_YELLOW
} TrafficLightState;

typedef struct {
    TrafficLightState state;
    DWORD lastChangeTime;
    int greenDuration;
    int yellowDuration;
    int redDuration;
} TrafficLight;

typedef struct VehicleNode {
    Vehicle vehicle;
    struct VehicleNode* next;
} VehicleNode;

typedef struct {
    VehicleNode* front;
    VehicleNode* rear;
    int count;
} VehicleQueue;

void initializeTrafficLight(TrafficLight* light, int greenDuration, int yellowDuration, int redDuration);
void updateTrafficLight(TrafficLight* light);
void enqueueVehicle(VehicleQueue* queue, Vehicle vehicle);
Vehicle dequeueVehicle(VehicleQueue* queue);
int isQueueEmpty(VehicleQueue* queue);

#endif // TRAFFIC_SYSTEM_H