#ifndef VEHICLE_H
#define VEHICLE_H

#include <SDL.h>
#include <stdbool.h>

typedef struct {
    char id[9];
    char lane;
    SDL_Point position;
    int speed;
    int waiting_time;
    bool is_moving;
} Vehicle;

Vehicle* vehicle_create(const char* id, char lane);
void vehicle_destroy(Vehicle* vehicle);
void vehicle_update_position(Vehicle* vehicle);
void vehicle_draw(SDL_Renderer* renderer, Vehicle* vehicle);

#endif