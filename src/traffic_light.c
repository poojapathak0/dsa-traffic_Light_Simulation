#include "traffic_light.h"

void traffic_light_init(TrafficLight* lights) {
    // Initialize traffic lights at the intersection, positioned on the side of the road
    int offset = 100; // Increased offset to move the lights further from the intersection
    lights[0] = (TrafficLight){400 - offset, 200, 0}; // North (left side)
    lights[1] = (TrafficLight){600, 400 - offset, 0}; // East (above)
    lights[2] = (TrafficLight){400 + offset, 600, 0}; // South (right side)
    lights[3] = (TrafficLight){200, 400 + offset, 0}; // West (below)
}

void traffic_light_update(TrafficLight* lights, int currentState) {
    // Update traffic light states based on the current state
    for (int i = 0; i < 4; i++) {
        lights[i].state = (currentState == 1 && (i == 0 || i == 2)) ||
                          (currentState == 2 && (i == 1 || i == 3));
    }
}

void traffic_light_draw(SDL_Renderer* renderer, TrafficLight* light) {
    // Draw the traffic light as three circles (or rectangles)
    int lightWidth = 20;
    int lightHeight = 60;
    int lightSpacing = 5;

    // Draw the traffic light body
    SDL_Rect body = {light->x - lightWidth / 2, light->y - lightHeight / 2, lightWidth, lightHeight};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark gray for the body
    SDL_RenderFillRect(renderer, &body);

    // Draw the lights
    for (int i = 0; i < 3; i++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Default to off (black)
        if (light->state == 1 && i == 0) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
        } else if (light->state == 2 && i == 1) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        } else if (light->state == 0 && i == 2) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        }

        // Draw the light
        SDL_Rect lightRect = {light->x - lightWidth / 4, light->y - lightHeight / 2 + i * (lightHeight / 3 + lightSpacing), lightWidth / 2, lightHeight / 3};
        SDL_RenderFillRect(renderer, &lightRect);
    }
}