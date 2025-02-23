#include <SDL.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "vehicle.h"
#include "traffic_light.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_VEHICLES 100

typedef struct {
    VehicleQueue queues[4];  // One queue per direction
    TrafficLight lights[4];  // Traffic lights for all directions
    int currentState;        // 0=all red, 1=A/C green, 2=B/D green
    Vehicle* activeVehicles[MAX_VEHICLES];
    int activeVehicleCount;
    pthread_mutex_t vehicleMutex;
} SharedData;

void drawRoadsAndLanes(SDL_Renderer* renderer);
void* processQueues(void* arg);
void* readVehicleData(void* arg);
bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    printf("Initializing SDL...\n");
    if (!initializeSDL(&window, &renderer)) {
        printf("Failed to initialize SDL.\n");
        return -1;
    }
    printf("SDL initialized successfully.\n");

    // Initialize shared data
    SharedData sharedData = {0};
    pthread_mutex_init(&sharedData.vehicleMutex, NULL);
    for(int i = 0; i < 4; i++) {
        queue_init(&sharedData.queues[i]);
    }
    traffic_light_init(sharedData.lights);

    // Create threads
    pthread_t queueThread, dataThread;
    pthread_create(&queueThread, NULL, processQueues, &sharedData);
    pthread_create(&dataThread, NULL, readVehicleData, &sharedData);

    // Main loop
    bool running = true;
    printf("Entering main loop...\n");
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw roads and lanes
        drawRoadsAndLanes(renderer);

        // Draw traffic lights
        for(int i = 0; i < 4; i++) {
            traffic_light_draw(renderer, &sharedData.lights[i]);
        }

        // Draw vehicles
        pthread_mutex_lock(&sharedData.vehicleMutex);
        for(int i = 0; i < sharedData.activeVehicleCount; i++) {
            if (sharedData.activeVehicles[i]) {
                vehicle_update_position(sharedData.activeVehicles[i]);
                vehicle_draw(renderer, sharedData.activeVehicles[i]);
            }
        }
        pthread_mutex_unlock(&sharedData.vehicleMutex);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    for(int i = 0; i < 4; i++) {
        queue_destroy(&sharedData.queues[i]);
    }
    pthread_mutex_destroy(&sharedData.vehicleMutex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Exiting...\n");
    return 0;
}

void* processQueues(void* arg) {
    SharedData* data = (SharedData*)arg;
    int cycleTime = 0;

    while(1) {
        // Update traffic light state
        cycleTime = (cycleTime + 1) % 20;
        if (cycleTime < 10) {
            data->currentState = 1; // A/C green
        } else {
            data->currentState = 2; // B/D green
        }
        traffic_light_update(data->lights, data->currentState);

        // Process queues based on light state
        pthread_mutex_lock(&data->vehicleMutex);
        for(int i = 0; i < 4; i++) {
            char direction = 'A' + i;
            bool canMove = (data->currentState == 1 && (direction == 'A' || direction == 'C')) ||
                          (data->currentState == 2 && (direction == 'B' || direction == 'D'));

            if (canMove && !queue_is_empty(&data->queues[i])) {
                Vehicle* vehicle = queue_dequeue(&data->queues[i]);
                if (vehicle && data->activeVehicleCount < MAX_VEHICLES) {
                    vehicle->is_moving = true;
                    data->activeVehicles[data->activeVehicleCount++] = vehicle;
                }
            }
        }
        pthread_mutex_unlock(&data->vehicleMutex);

        usleep(100000); // 100ms delay
    }
    return NULL;
}

void* readVehicleData(void* arg) {
    SharedData* data = (SharedData*)arg;
    FILE* file;
    char line[20];
    
    while(1) {
        file = fopen("vehicles.data", "r");
        if (!file) {
            perror("Error opening vehicles.data");
            sleep(1);
            continue;
        }

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;
            char* id = strtok(line, ":");
            char* laneStr = strtok(NULL, ":");
            
            if (id && laneStr) {
                char lane = laneStr[0];
                int queueIndex = lane - 'A';
                if (queueIndex >= 0 && queueIndex < 4) {
                    Vehicle* vehicle = vehicle_create(id, lane);
                    queue_enqueue(&data->queues[queueIndex], vehicle);
                }
            }
        }
        
        fclose(file);
        sleep(1);
    }
    return NULL;
}

void drawRoadsAndLanes(SDL_Renderer* renderer) {
    // Road background
    SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
    SDL_Rect verticalRoad = {WINDOW_WIDTH/2 - 75, 0, 150, WINDOW_HEIGHT};
    SDL_Rect horizontalRoad = {0, WINDOW_HEIGHT/2 - 75, WINDOW_WIDTH, 150};
    SDL_RenderFillRect(renderer, &verticalRoad);
    SDL_RenderFillRect(renderer, &horizontalRoad);

    // Lane markings
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(int i = 0; i <= 3; i++) {
        int offset = i * 50;
        // Vertical lanes
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH/2 - 75 + offset, 0,
                          WINDOW_WIDTH/2 - 75 + offset, WINDOW_HEIGHT);
        // Horizontal lanes
        SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT/2 - 75 + offset,
                          WINDOW_WIDTH, WINDOW_HEIGHT/2 - 75 + offset);
    }
}

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Traffic Junction Simulator",
                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return false;
    }

    return true;
}