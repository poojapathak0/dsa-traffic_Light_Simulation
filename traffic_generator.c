#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LANES 4
#define MAX_VEHICLES 20

void generate_traffic() {
    const char *lane_files[NUM_LANES] = {"laneA.txt", "laneB.txt", "laneC.txt", "laneD.txt"};
    srand(time(NULL));

    while (1) {
        for (int i = 0; i < NUM_LANES; i++) {
            FILE *file = fopen(lane_files[i], "w");
            if (file) {
                int vehicles = rand() % MAX_VEHICLES;
                fprintf(file, "%d\n", vehicles);
                fclose(file);
            }
        }
        sleep(1); // Wait for 1 second before generating new traffic
    }
}

int main() {
    generate_traffic();
    return 0;
}