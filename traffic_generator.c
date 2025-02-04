#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_ROADS 4
#define MAX_VEHICLES 20

int lanes[NUM_ROADS];

void generateVehicles() {
    for (int i = 0; i < NUM_ROADS; i++) {
        if (lanes[i] < MAX_VEHICLES) {
            lanes[i] += rand() % 3; // Add 0 to 2 vehicles randomly
            if (lanes[i] > MAX_VEHICLES) {
                lanes[i] = MAX_VEHICLES; // Cap at max vehicles
            }
        }
    }
}

int main() {
    srand(time(NULL));

    // Initialize lanes
    for (int i = 0; i < NUM_ROADS; i++) {
        lanes[i] = 0; // Start with 0 vehicles
    }

    while (1) {
        generateVehicles();
        printf("Current vehicles in lanes: A: %d, B: %d, C: %d, D: %d\n", lanes[0], lanes[1], lanes[2], lanes[3]);
        sleep(1); // Generate vehicles every second
    }

    return 0;
}