#ifndef VEHICLE_H
#define VEHICLE_H

#include <SDL2/SDL.h>
#include "traffic_light_simulator.h"

// Vehicle structure
typedef struct Vehicle {
    VehicleType type;
    SDL_Rect position;
    Direction direction;
    struct Vehicle* next;
} Vehicle;

#endif
