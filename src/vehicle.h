#ifndef VEHICLE_H
#define VEHICLE_H

#include <SDL.h>
#include "common_types.h"

// Vehicle structure
typedef struct Vehicle {
    VehicleType type;
    SDL_Rect position;
    Direction direction;
    struct Vehicle* next;
} Vehicle;

#endif