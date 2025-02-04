// traffic_light_simulator.c
#include "traffic_light_simulator.h"
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Color definitions
static const SDL_Color ROAD_COLOR = {80, 80, 80, 255};
static const SDL_Color GRASS_COLOR = {34, 139, 34, 255};
static const SDL_Color MARKING_COLOR = {255, 255, 255, 255};

// Light timing constants
#define GREEN_TIME 150
#define YELLOW_TIME 30
#define RED_TIME 180
#define PRIORITY_THRESHOLD 10

bool init_simulator(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Traffic Light Simulator",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             WINDOW_WIDTH, WINDOW_HEIGHT,
                             SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1,
                                 SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void init_traffic_lights(TrafficLight* lights) {
    // Position traffic lights at each direction
    SDL_Point positions[4] = {
        {WINDOW_WIDTH/2 - ROAD_WIDTH, WINDOW_HEIGHT/2 - ROAD_WIDTH},  // North
        {WINDOW_WIDTH/2 + ROAD_WIDTH, WINDOW_HEIGHT/2 - ROAD_WIDTH},  // East
        {WINDOW_WIDTH/2 + ROAD_WIDTH, WINDOW_HEIGHT/2 + ROAD_WIDTH},  // South
        {WINDOW_WIDTH/2 - ROAD_WIDTH, WINDOW_HEIGHT/2 + ROAD_WIDTH}   // West
    };

    Direction directions[4] = {NORTH, EAST, SOUTH, WEST};

    for (int i = 0; i < 4; i++) {
        lights[i].state = RED;
        lights[i].timer = RED_TIME;
        lights[i].direction = directions[i];
        lights[i].isPriority = (i == 1); // East is priority direction (AL2)
        lights[i].waitingVehicles = 0;
        lights[i].position = (SDL_Rect){
            positions[i].x,
            positions[i].y,
            LIGHT_SIZE,
            LIGHT_SIZE * 3
        };
    }
}

void render_traffic_lights(SDL_Renderer* renderer, TrafficLight* lights) {
    for (int i = 0; i < 4; i++) {
        TrafficLight light = lights[i];
        
        // Draw light housing
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &light.position);
        
        // Draw individual lights
        SDL_Rect lightBulb = {
            light.position.x + 5,
            light.position.y + 5,
            LIGHT_SIZE - 10,
            LIGHT_SIZE - 10
        };
        
        // Red light
        SDL_SetRenderDrawColor(renderer, 
            light.state == RED ? 255 : 50,
            0, 0, 255);
        SDL_RenderFillRect(renderer, &lightBulb);
        
        // Yellow light
        lightBulb.y += LIGHT_SIZE;
        SDL_SetRenderDrawColor(renderer,
            light.state == YELLOW ? 255 : 50,
            light.state == YELLOW ? 255 : 50,
            0, 255);
        SDL_RenderFillRect(renderer, &lightBulb);
        
        // Green light
        lightBulb.y += LIGHT_SIZE;
        SDL_SetRenderDrawColor(renderer,
            0,
            light.state == GREEN ? 255 : 50,
            0, 255);
        SDL_RenderFillRect(renderer, &lightBulb);
    }
}

void render_road_system(SDL_Renderer* renderer) {
    // Draw grass background
    SDL_SetRenderDrawColor(renderer, 
        GRASS_COLOR.r, GRASS_COLOR.g, GRASS_COLOR.b, GRASS_COLOR.a);
    SDL_RenderClear(renderer);
    
    // Draw roads
    SDL_SetRenderDrawColor(renderer, 
        ROAD_COLOR.r, ROAD_COLOR.g, ROAD_COLOR.b, ROAD_COLOR.a);
    
    // Horizontal road
    SDL_Rect horizontal_road = {
        0,
        (WINDOW_HEIGHT - ROAD_WIDTH * 2) / 2,
        WINDOW_WIDTH,
        ROAD_WIDTH * 2
    };
    SDL_RenderFillRect(renderer, &horizontal_road);
    
    // Vertical road
    SDL_Rect vertical_road = {
        (WINDOW_WIDTH - ROAD_WIDTH * 2) / 2,
        0,
        ROAD_WIDTH * 2,
        WINDOW_HEIGHT
    };
    SDL_RenderFillRect(renderer, &vertical_road);
}

void render_lane_markings(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 
        MARKING_COLOR.r, MARKING_COLOR.g, MARKING_COLOR.b, MARKING_COLOR.a);
    
    // Horizontal lane markings
    for (int x = 0; x < WINDOW_WIDTH; x += 40) {
        SDL_Rect dash = {
            x,
            WINDOW_HEIGHT / 2,
            20,
            3
        };
        SDL_RenderFillRect(renderer, &dash);
    }
    
    // Vertical lane markings
    for (int y = 0; y < WINDOW_HEIGHT; y += 40) {
        SDL_Rect dash = {
            WINDOW_WIDTH / 2,
            y,
            3,
            20
        };
        SDL_RenderFillRect(renderer, &dash);
    }
}

void init_lane_queues(LaneQueue* queues) {
    Direction directions[4] = {NORTH, EAST, SOUTH, WEST};
    for (int i = 0; i < 4; i++) {
        queues[i].vehicleCount = 0;
        queues[i].front = NULL;
        queues[i].rear = NULL;
        queues[i].direction = directions[i];
    }
}

void update_traffic_lights(TrafficLight* lights) {
    for (int i = 0; i < 4; i++) {
        lights[i].timer--;
        if (lights[i].timer <= 0) {
            switch (lights[i].state) {
                case GREEN:
                    lights[i].state = YELLOW;
                    lights[i].timer = YELLOW_TIME;
                    break;
                case YELLOW:
                    lights[i].state = RED;
                    lights[i].timer = RED_TIME;
                    break;
                case RED:
                    lights[i].state = GREEN;
                    lights[i].timer = GREEN_TIME;
                    break;
            }
        }
    }
}

void update_vehicles(LaneQueue* queues, TrafficLight* lights) {
    for (int i = 0; i < 4; i++) {
        if (lights[i].state == GREEN && queues[i].front != NULL) {
            // Remove vehicle from front of queue
            Vehicle* temp = queues[i].front;
            queues[i].front = queues[i].front->next;
            free(temp);
            queues[i].vehicleCount--;
            
            if (queues[i].front == NULL) {
                queues[i].rear = NULL;
            }
        }
    }
}

void handle_priority_signals(TrafficLight* lights) {
    for (int i = 0; i < 4; i++) {
        if (lights[i].isPriority && lights[i].waitingVehicles > PRIORITY_THRESHOLD) {
            // Implement priority handling logic
            lights[i].timer = MIN(lights[i].timer, 30); // Force faster transition
        }
    }
}

void render_vehicles(SDL_Renderer* renderer, LaneQueue* queues) {
    // Basic vehicle rendering
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int i = 0; i < 4; i++) {
        // Add vehicle rendering logic here
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    if (!init_simulator(&window, &renderer)) {
        return 1;
    }
    
    TrafficLight lights[4];
    init_traffic_lights(lights);
    
    LaneQueue queues[4];
    init_lane_queues(queues);
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Update simulation
        update_traffic_lights(lights);
        update_vehicles(queues, lights);
        handle_priority_signals(lights);
        
        // Render
        render_road_system(renderer);
        render_lane_markings(renderer);
        render_vehicles(renderer, queues);
        render_traffic_lights(renderer, lights);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Cap at ~60 FPS
    }
    
    cleanup_simulator(window, renderer);
    return 0;
}

void cleanup_simulator(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}