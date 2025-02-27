#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "vehicle.h"

#define FILENAME "vehicles.data"
#define MIN_TIME_GAP 30000 // Minimum time gap between vehicles in the same lane (in milliseconds)

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

int willTurnLeft() {
    return rand() % 2; // 50% chance to turn left
}

int main(int argc, char *argv[]) {
    // Set console code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

    int laneCounts[4] = {0, 0, 0, 0}; // Initialize vehicle counts for each lane
    CRITICAL_SECTION cs[4]; // Critical sections for each lane
    for (int i = 0; i < 4; i++) {
        InitializeCriticalSection(&cs[i]);
    }

    DWORD lastVehicleTime[4] = {0, 0, 0, 0}; // Last vehicle generation time for each lane

    while (1) {
        char vehicle[9];
        generateVehicleNumber(vehicle);
        char lane = generateLane();
        VehicleType type = generateVehicleType();
        int turnLeft = willTurnLeft(); // Determine if the vehicle will turn left
    
        int laneIndex = lane - 'A';
        EnterCriticalSection(&cs[laneIndex]); // Enter critical section for the lane

        DWORD currentTime = GetTickCount();
        if (currentTime - lastVehicleTime[laneIndex] < MIN_TIME_GAP) {
            LeaveCriticalSection(&cs[laneIndex]); // Leave critical section for the lane
            Sleep(MIN_TIME_GAP - (currentTime - lastVehicleTime[laneIndex])); // Wait for the remaining time gap
            continue;
        }
    
        FILE* file = fopen(FILENAME, "a");
        if (!file) {
            perror("Error opening file");
            LeaveCriticalSection(&cs[laneIndex]); // Leave critical section for the lane
            return 1;
        }
    
        // Log the vehicle's action, including left turn if applicable
        fprintf(file, "%s:%c:%s:%s\n", vehicle, lane, vehicleTypeToString(type), turnLeft ? "LEFT" : "STRAIGHT");
        fflush(file);
        fclose(file);
    
        printf("Generated: %s:%c:%s:%s\n", vehicle, lane, vehicleTypeToString(type), turnLeft ? "LEFT" : "STRAIGHT");
    
        // Update lane vehicle count
        laneCounts[laneIndex]++;
        lastVehicleTime[laneIndex] = currentTime; // Update last vehicle generation time
    
        // Increase sleep duration to 3000ms (3 seconds) to reduce vehicle generation frequency
        Sleep(3000);
    
        LeaveCriticalSection(&cs[laneIndex]); // Leave critical section for the lane
    }

    for (int i = 0; i < 4; i++) {
        DeleteCriticalSection(&cs[i]); // Delete critical sections for each lane
    }

    return 0;
}