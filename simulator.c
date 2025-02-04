#include "include/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NUM_ROADS 4
#define MAX_VEHICLES 20

typedef struct {
    int waitingVehicles;
    bool isPriority;
} Lane;

Lane lanes[NUM_ROADS];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void initializeSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Traffic Light Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void cleanupSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderTrafficLights() {
    // Render traffic lights for each road
    for (int i = 0; i < NUM_ROADS; i++) {
        // Set color based on light state
        SDL_SetRenderDrawColor(renderer, lanes[i].isPriority ? 0 : 255, lanes[i].isPriority ? 255 : 0, 0, 255); // Green for priority, red otherwise
        SDL_Rect lightRect = { i * (WINDOW_WIDTH / NUM_ROADS), 50, (WINDOW_WIDTH / NUM_ROADS) - 10, 100 };
        SDL_RenderFillRect(renderer, &lightRect);
    }
}

void processTraffic() {
    // Check for priority conditions
    for (int i = 0; i < NUM_ROADS; i++) {
        if (lanes[i].waitingVehicles > 10) {
            lanes[i].isPriority = true;
        } else {
            lanes[i].isPriority = false;
        }
    }

    // Simulate vehicle passing
    for (int i = 0; i < NUM_ROADS; i++) {
        if (lanes[i].isPriority && lanes[i].waitingVehicles > 0) {
            lanes[i].waitingVehicles--;
        }
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    renderTrafficLights();
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    initializeSDL();

    // Initialize lanes
    for (int i = 0; i < NUM_ROADS; i++) {
        lanes[i].waitingVehicles = rand() % MAX_VEHICLES; // Random initial vehicles
        lanes[i].isPriority = false;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        processTraffic();
        render();
        SDL_Delay(1000); // Update every second
    }

    cleanupSDL();
    return 0;
}