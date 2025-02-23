#include "vehicle.h"
#include <stdlib.h>
#include <string.h>

#define VEHICLE_WIDTH 30
#define VEHICLE_HEIGHT 50
#define MOVEMENT_SPEED 2

Vehicle* vehicle_create(const char* id, char lane, VehicleType type) {
    Vehicle* vehicle = (Vehicle*)malloc(sizeof(Vehicle));
    if (vehicle) {
        strncpy(vehicle->id, id, sizeof(vehicle->id) - 1);
        vehicle->id[sizeof(vehicle->id) - 1] = '\0';
        vehicle->lane = lane;
        vehicle->type = type;
        vehicle->is_moving = false;

        // Set initial positions based on lane
        switch (lane) {
            case 'A': // North to South
                vehicle->x = 400 - VEHICLE_WIDTH/2;
                vehicle->y = 0;
                break;
            case 'B': // East to West
                vehicle->x = 800 - VEHICLE_WIDTH;
                vehicle->y = 400 - VEHICLE_HEIGHT/2;
                break;
            case 'C': // South to North
                vehicle->x = 400 + VEHICLE_WIDTH/2;
                vehicle->y = 800 - VEHICLE_HEIGHT;
                break;
            case 'D': // West to East
                vehicle->x = 0;
                vehicle->y = 400 + VEHICLE_HEIGHT/2;
                break;
        }
    }
    return vehicle;
}

void vehicle_update_position(Vehicle* vehicle) {
    if (!vehicle->is_moving) return;

    switch (vehicle->lane) {
        case 'A': // Moving South
            vehicle->y += MOVEMENT_SPEED;
            if (vehicle->y > 800) {
                vehicle->is_moving = false;
            }
            break;
        case 'B': // Moving West
            vehicle->x -= MOVEMENT_SPEED;
            if (vehicle->x < -VEHICLE_WIDTH) {
                vehicle->is_moving = false;
            }
            break;
        case 'C': // Moving North
            vehicle->y -= MOVEMENT_SPEED;
            if (vehicle->y < -VEHICLE_HEIGHT) {
                vehicle->is_moving = false;
            }
            break;
        case 'D': // Moving East
            vehicle->x += MOVEMENT_SPEED;
            if (vehicle->x > 800) {
                vehicle->is_moving = false;
            }
            break;
    }
}

void vehicle_draw(SDL_Renderer* renderer, Vehicle* vehicle) {
    // Set color based on vehicle type
    switch (vehicle->type) {
        case NORMAL:
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
            break;
        case VIP:
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Gold
            break;
        case AMBULANCE:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            break;
    }

    // Draw main vehicle body
    SDL_Rect body = {vehicle->x, vehicle->y, VEHICLE_WIDTH, VEHICLE_HEIGHT};
    SDL_RenderFillRect(renderer, &body);

    // Draw vehicle details (windows)
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light gray
    
    // Top window
    SDL_Rect window1 = {
        vehicle->x + 5,
        vehicle->y + 5,
        VEHICLE_WIDTH - 10,
        10
    };
    SDL_RenderFillRect(renderer, &window1);
    
    // Bottom window
    SDL_Rect window2 = {
        vehicle->x + 5,
        vehicle->y + VEHICLE_HEIGHT - 15,
        VEHICLE_WIDTH - 10,
        10
    };
    SDL_RenderFillRect(renderer, &window2);

    // Special markings for ambulance
    if (vehicle->type == AMBULANCE) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect cross_v = {
            vehicle->x + VEHICLE_WIDTH/2 - 2,
            vehicle->y + VEHICLE_HEIGHT/4,
            4,
            VEHICLE_HEIGHT/3
        };
        SDL_Rect cross_h = {
            vehicle->x + VEHICLE_WIDTH/4,
            vehicle->y + VEHICLE_HEIGHT/2 - 2,
            VEHICLE_WIDTH/2,
            4
        };
        SDL_RenderFillRect(renderer, &cross_v);
        SDL_RenderFillRect(renderer, &cross_h);
    }
}

void vehicle_destroy(Vehicle* vehicle) {
    free(vehicle);
}