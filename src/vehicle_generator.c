// vehicle_generator.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define GENERATION_INTERVAL 2
#define NUM_LANES 4
#define PRIORITY_LANE 1      

void generate_vehicle(int lane_id) {
    char filename[20];
    sprintf(filename, "lane%d.txt", lane_id);
    
    FILE* file = fopen(filename, "a");
    if (file) {
        // Generate random vehicle type (0: CAR, 1: TRUCK, 2: BIKE)
        int vehicle_type = rand() % 3;
        // Generate random direction (0: straight, 1: left, 2: right)
        int direction = rand() % 3;
        
        fprintf(file, "%d,%d\n", vehicle_type, direction);
        fclose(file);
    }
}

int SDL_main(int argc, char *argv[]) {
    srand(time(NULL));
    
    while (1) {
        // Generate vehicles with different probabilities for each lane
        for (int lane = 0; lane < NUM_LANES; lane++) {
            int probability = (lane == PRIORITY_LANE) ? 40 : 30;  // Higher probability for priority lane
            
            if (rand() % 100 < probability) {
                generate_vehicle(lane);
            }
        }
        
        sleep(GENERATION_INTERVAL);
    }
    
    return 0;
}