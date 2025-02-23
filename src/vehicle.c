#include "vehicle.h"
#include <stdlib.h>
#include <string.h>

#define VEHICLE_SIZE 20
#define VEHICLE_SPEED 2

Vehicle* vehicle_create(const char* id, char lane) {
    Vehicle* vehicle = (Vehicle*)malloc(sizeof(Vehicle));
    strncpy(vehicle->id, id, 8);
    vehicle->id[8] = '\0';
    vehicle->lane = lane;
    vehicle->speed = VEHICLE_SPEED;
    vehicle->waiting_time = 0;
    vehicle->is_moving = false;

    switch(lane) {
        case 'A':
            vehicle->position.x = 400;
            vehicle->position.y = 0;
            break;
        case 'B':
            vehicle->position.x = 400;
            vehicle->position.y = 800;
            break;
        case 'C':
            vehicle->position.x = 800;
            vehicle->position.y = 400;
            break;
        case 'D':
            vehicle->position.x = 0;
            vehicle->position.y = 400;
            break;
    }

    return vehicle;
}

void vehicle_destroy(Vehicle* vehicle) {
    free(vehicle);
}

void vehicle_update_position(Vehicle* vehicle) {
    if (!vehicle->is_moving) return;

    switch(vehicle->lane) {
        case 'A':
            vehicle->position.y += vehicle->speed;
            break;
        case 'B':
            vehicle->position.y -= vehicle->speed;
            break;
        case 'C':
            vehicle->position.x -= vehicle->speed;
            break;
        case 'D':
            vehicle->position.x += vehicle->speed;
            break;
    }
}

void vehicle_draw(SDL_Renderer* renderer, Vehicle* vehicle) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect = {
        vehicle->position.x - VEHICLE_SIZE/2,
        vehicle->position.y - VEHICLE_SIZE/2,
        VEHICLE_SIZE,
        VEHICLE_SIZE
    };
    SDL_RenderFillRect(renderer, &rect);
}