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

int main() {
    // Set console code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

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
        Sleep(1000);
    }

    return 0;
}
