#include <SDL.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "priority_queue.h"
#include "queue.h"
#include "vehicle.h"
#include "traffic_light.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_VEHICLES 100

typedef struct {
    PriorityQueue queues[4];  // Priority queues for all directions
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
    // Set up Windows console for proper output
    SetConsoleOutputCP(CP_UTF8);
    
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    printf("Initializing SDL...\n");
    if (!initializeSDL(&window, &renderer)) {
        printf("Failed to initialize SDL.\n");
        return -1;
    }
    printf("SDL initialized successfully.\n");

    SharedData sharedData = {0};
    pthread_mutex_init(&sharedData.vehicleMutex, NULL);
    for(int i = 0; i < 4; i++) {
        priority_queue_init(&sharedData.queues[i]);
    }
    traffic_light_init(sharedData.lights);

    pthread_t queueThread, dataThread;
    pthread_create(&queueThread, NULL, processQueues, &sharedData);
    pthread_create(&dataThread, NULL, readVehicleData, &sharedData);

    bool running = true;
    printf("Entering main loop...\n");
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawRoadsAndLanes(renderer);

        for(int i = 0; i < 4; i++) {
            traffic_light_draw(renderer, &sharedData.lights[i]);
        }

        pthread_mutex_lock(&sharedData.vehicleMutex);
        for(int i = 0; i < sharedData.activeVehicleCount; i++) {
            if (sharedData.activeVehicles[i]) {
                vehicle_update_position(sharedData.activeVehicles[i]);
                vehicle_draw(renderer, sharedData.activeVehicles[i]);
            }
        }
        pthread_mutex_unlock(&sharedData.vehicleMutex);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    for(int i = 0; i < 4; i++) {
        priority_queue_destroy(&sharedData.queues[i]);
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
        cycleTime = (cycleTime + 1) % 20;
        if (cycleTime < 10) {
            data->currentState = 1; // A/C green
        } else {
            data->currentState = 2; // B/D green
        }
        traffic_light_update(data->lights, data->currentState);

        pthread_mutex_lock(&data->vehicleMutex);
        for(int i = 0; i < 4; i++) {
            char direction = 'A' + i;
            bool canMove = (data->currentState == 1 && (direction == 'A' || direction == 'C')) ||
                          (data->currentState == 2 && (direction == 'B' || direction == 'D'));

            if (canMove && !priority_queue_is_empty(&data->queues[i])) {
                Vehicle* vehicle = priority_queue_dequeue(&data->queues[i]);
                if (vehicle && data->activeVehicleCount < MAX_VEHICLES) {
                    vehicle->is_moving = true;
                    data->activeVehicles[data->activeVehicleCount++] = vehicle;
                }
            }
        }
        pthread_mutex_unlock(&data->vehicleMutex);

        Sleep(100); // Windows sleep function
    }
    return NULL;
}

void* readVehicleData(void* arg) {
    SharedData* data = (SharedData*)arg;
    FILE* file;
    char line[50];
    
    while(1) {
        file = fopen("vehicles.data", "r");
        if (!file) {
            perror("Error opening vehicles.data");
            Sleep(1000); // Windows sleep function
            continue;
        }

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;
            char* id = strtok(line, ":");
            char* laneStr = strtok(NULL, ":");
            char* typeStr = strtok(NULL, ":");
            
            if (id && laneStr && typeStr) {
                char lane = laneStr[0];
                VehicleType type;
                if (strcmp(typeStr, "NORMAL") == 0) {
                    type = NORMAL;
                } else if (strcmp(typeStr, "VIP") == 0) {
                    type = VIP;
                } else if (strcmp(typeStr, "AMBULANCE") == 0) {
                    type = AMBULANCE;
                }
                int queueIndex = lane - 'A';  // Adjust based on lane configuration
                if (queueIndex >= 0 && queueIndex < 4) {
                    Vehicle* vehicle = vehicle_create(id, lane, type);
                    priority_queue_enqueue(&data->queues[queueIndex], vehicle);
                    printf("Read vehicle: %s in lane: %c with type: %s\n", id, lane, typeStr); // Debug print statement
                }
            }
        }
        
        fclose(file);
        Sleep(1000); // Windows sleep function
    }
    return NULL;
}

void drawRoadsAndLanes(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
    SDL_Rect verticalRoad = {WINDOW_WIDTH/2 - 75, 0, 150, WINDOW_HEIGHT};
    SDL_Rect horizontalRoad = {0, WINDOW_HEIGHT/2 - 75, WINDOW_WIDTH, 150};
    SDL_RenderFillRect(renderer, &verticalRoad);
    SDL_RenderFillRect(renderer, &horizontalRoad);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(int i = 0; i <= 3; i++) {
        int offset = i * 50;
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH/2 - 75 + offset, 0,
                          WINDOW_WIDTH/2 - 75 + offset, WINDOW_HEIGHT);
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