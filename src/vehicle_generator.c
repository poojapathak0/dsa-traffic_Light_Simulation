#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "vehicle.h"

#define FILENAME "vehicles.data"

void generateVehicleNumber(char* buffer) {
    buffer[0] = 'A' + rand() % 26;
    buffer[1] = 'A' + rand() % 26;
    buffer[2] = '0' + rand() % 10;
    buffer[3] = 'A' + rand() % 26;
    buffer[4] = 'A' + rand() % 26;
    buffer[5] = '0' + rand() % 10;
    buffer[6] = '0' + rand() % 10;
    buffer[7] = '0' + rand() % 10;
    buffer[8] = '\0';
}

char generateLane() {
    char lanes[] = {'A', 'B', 'C', 'D'};
    return lanes[rand() % 4];
}

const char* vehicleTypeToString(VehicleType type) {
    switch (type) {
        case NORMAL: return "NORMAL";
        case VIP: return "VIP";
        case AMBULANCE: return "AMBULANCE";
        default: return "UNKNOWN";
    }
}

VehicleType generateVehicleType() {
    int r = rand() % 100;
    if (r < 70) return NORMAL;
    if (r < 90) return VIP;
    return AMBULANCE;
}

int main(int argc, char *argv[]) {
    // Set console code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

    int laneCounts[4] = {0, 0, 0, 0}; // Initialize vehicle counts for each lane

    while (1) {
        char vehicle[9];
        generateVehicleNumber(vehicle);
        char lane = generateLane();
        VehicleType type = generateVehicleType();

        FILE* file = fopen(FILENAME, "a");
        if (!file) {
            perror("Error opening file");
            return 1;
        }

        fprintf(file, "%s:%c:%s\n", vehicle, lane, vehicleTypeToString(type));
        fflush(file);
        fclose(file);

        printf("Generated: %s:%c:%s\n", vehicle, lane, vehicleTypeToString(type));

        // Update lane vehicle count
        int laneIndex = lane - 'A';
        laneCounts[laneIndex]++;

        // Adjust sleep duration based on vehicle type and lane vehicle count
        if (type == AMBULANCE) {
            Sleep(50); // Ambulance has higher speed, so shorter sleep duration
        } else {
            if (laneCounts[laneIndex] >= 10) {
                Sleep(500); // Higher priority for lanes with 10 or more vehicles
            } else {
                Sleep(1000); // Normal and VIP vehicles have standard speed
            }
        }

        // Decrease count to simulate vehicle passing
        laneCounts[laneIndex]--;
    }

    return 0;
}