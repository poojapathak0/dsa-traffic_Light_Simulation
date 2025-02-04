#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_LANES 4
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    int vehicles;
    int priority;
} Lane;

Lane lanes[NUM_LANES];
const char *lane_files[NUM_LANES] = {"laneA.txt", "laneB.txt", "laneC.txt", "laneD.txt"};

void read_traffic_data() {
    for (int i = 0; i < NUM_LANES; i++) {
        FILE *file = fopen(lane_files[i], "r");
        if (file) {
            fscanf(file, "%d", &lanes[i].vehicles);
            fclose(file);
        }
    }
}

void update_priority() {
    for (int i = 0; i < NUM_LANES; i++) {
        if (lanes[i].vehicles > 10) {
            lanes[i].priority = 1;
        } else if (lanes[i].vehicles < 5) {
            lanes[i].priority = 0;
        }
    }
}

void render_traffic(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < NUM_LANES; i++) {
        SDL_Rect rect = {i * 200, 0, 200, lanes[i].vehicles * 20};
        SDL_SetRenderDrawColor(renderer, lanes[i].priority ? 255 : 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Traffic Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    while (1) {
        read_traffic_data();
        update_priority();
        render_traffic(renderer);
        SDL_Delay(1000); // Update every 1 second
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}