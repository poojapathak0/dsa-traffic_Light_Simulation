#include "traffic_light.h"

void traffic_light_init(TrafficLight* lights) {
    // Initialize traffic lights at the intersection
    lights[0] = (TrafficLight){400, 200, 0}; // North
    lights[1] = (TrafficLight){600, 400, 0}; // East
    lights[2] = (TrafficLight){400, 600, 0}; // South
    lights[3] = (TrafficLight){200, 400, 0}; // West
}

void traffic_light_update(TrafficLight* lights, int currentState) {
    // Update traffic light states based on the current state
    for (int i = 0; i < 4; i++) {
        lights[i].state = (currentState == 1 && (i == 0 || i == 2)) ||
                          (currentState == 2 && (i == 1 || i == 3));
    }
}

void traffic_light_draw(SDL_Renderer* renderer, TrafficLight* light) {
    SDL_Rect rect = {light->x - 10, light->y - 10, 20, 20};
    if (light->state == 1) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    }
    SDL_RenderFillRect(renderer, &rect);
}