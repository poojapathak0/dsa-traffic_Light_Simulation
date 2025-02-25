#ifndef VEHICLE_H
#define VEHICLE_H
#include <stdbool.h>
#include <SDL.h>
#include "traffic_light.h"

typedef enum {
    NORMAL,
    VIP,
    AMBULANCE
} VehicleType;

typedef struct {
    char id[9];
    char lane;
    VehicleType type;
    int x, y;
    bool is_moving;
} Vehicle;

Vehicle* vehicle_create(const char* id, char lane, VehicleType type);
void vehicle_update_position(Vehicle* vehicle, TrafficLight* lights);
void vehicle_draw(SDL_Renderer* renderer, Vehicle* vehicle);
void vehicle_destroy(Vehicle* vehicle);
bool is_light_green_for_lane(TrafficLight* lights, char lane);

#endif // VEHICLE_H