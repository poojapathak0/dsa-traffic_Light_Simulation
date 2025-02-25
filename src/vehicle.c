#include "vehicle.h"
#include <stdlib.h>
#include <string.h>

#define VEHICLE_WIDTH 30
#define VEHICLE_HEIGHT 50
#define MOVEMENT_SPEED 2

// Define stopping points for each lane
#define STOP_POINT_A_Y 350
#define STOP_POINT_B_X 700
#define STOP_POINT_C_Y 450
#define STOP_POINT_D_X 100

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

bool is_light_green_for_lane(TrafficLight* lights, char lane) {
    switch (lane) {
        case 'A': return lights[0].state == 1;
        case 'B': return lights[1].state == 1;
        case 'C': return lights[2].state == 1;
        case 'D': return lights[3].state == 1;
        default: return false;
    }
}

bool is_near_stop_point(Vehicle* vehicle) {
    switch (vehicle->lane) {
        case 'A': return vehicle->y >= STOP_POINT_A_Y - MOVEMENT_SPEED && vehicle->y <= STOP_POINT_A_Y + MOVEMENT_SPEED;
        case 'B': return vehicle->x <= STOP_POINT_B_X + MOVEMENT_SPEED && vehicle->x >= STOP_POINT_B_X - MOVEMENT_SPEED;
        case 'C': return vehicle->y <= STOP_POINT_C_Y + MOVEMENT_SPEED && vehicle->y >= STOP_POINT_C_Y - MOVEMENT_SPEED;
        case 'D': return vehicle->x >= STOP_POINT_D_X - MOVEMENT_SPEED && vehicle->x <= STOP_POINT_D_X + MOVEMENT_SPEED;
        default: return false;
    }
}

void vehicle_update_position(Vehicle* vehicle, TrafficLight* lights) {
    if (!vehicle->is_moving) return;

    // Allow ambulances to pass regardless of traffic light state
    if (vehicle->type != AMBULANCE) {
        // Check if the vehicle is near its stopping point and the light is red
        if (is_near_stop_point(vehicle) && !is_light_green_for_lane(lights, vehicle->lane)) {
            return; // Stop the vehicle if the light is red
        }
    }

    // Move the vehicle
    switch (vehicle->lane) {
        case 'A': vehicle->y += MOVEMENT_SPEED; break; // Moving South
        case 'B': vehicle->x -= MOVEMENT_SPEED; break; // Moving West
        case 'C': vehicle->y -= MOVEMENT_SPEED; break; // Moving North
        case 'D': vehicle->x += MOVEMENT_SPEED; break; // Moving East
    }

    // Check if vehicle has exited the screen
    if (vehicle->x < -VEHICLE_WIDTH || vehicle->x > 800 || vehicle->y < -VEHICLE_HEIGHT || vehicle->y > 800) {
        vehicle->is_moving = false;
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