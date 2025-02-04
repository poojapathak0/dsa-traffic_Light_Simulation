// traffic_light_simulator.h
#ifndef TRAFFIC_LIGHT_SIMULATOR_H
#define TRAFFIC_LIGHT_SIMULATOR_H

#include <SDL.h>
#include <stdbool.h>
#include <math.h>

// Window and display constants
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define VEHICLE_SIZE 30
#define ROAD_WIDTH 80
#define LIGHT_SIZE 40

// Traffic light states
typedef enum {
    RED,
    YELLOW,
    GREEN
} LightState;

// Vehicle types
typedef enum {
    CAR,
    TRUCK,
    BIKE
} VehicleType;

// Direction enumeration
typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

// Traffic Light structure
typedef struct {
    LightState state;
    int timer;
    SDL_Rect position;
    Direction direction;
    bool isPriority;
    int waitingVehicles;
} TrafficLight;

// Vehicle structure
typedef struct {
    float x, y;
    float target_x, target_y;
    float speed;
    float angle;
    VehicleType type;
    Direction direction;
    SDL_Color color;
    bool isWaiting;
} Vehicle;

// Lane Queue structure
typedef struct {
    Vehicle* vehicles;
    int capacity;
    int size;
    int front;
    int rear;
    Direction direction;
    bool isPriority;
} LaneQueue;

// Function declarations
bool init_simulator(SDL_Window** window, SDL_Renderer** renderer);
void cleanup_simulator(SDL_Window* window, SDL_Renderer* renderer);

// Traffic light functions
void init_traffic_lights(TrafficLight* lights);
void update_traffic_lights(TrafficLight* lights);
void render_traffic_lights(SDL_Renderer* renderer, TrafficLight* lights);
void handle_priority_signals(TrafficLight* lights);

// Vehicle and queue functions
void init_lane_queues(LaneQueue* queues);
bool add_vehicle_to_lane(LaneQueue* queue, Vehicle vehicle);
bool remove_vehicle_from_lane(LaneQueue* queue, Vehicle* vehicle);
void update_vehicles(LaneQueue* queues, TrafficLight* lights);
void render_vehicles(SDL_Renderer* renderer, LaneQueue* queues);

// Road and junction functions
void render_road_system(SDL_Renderer* renderer);
void render_lane_markings(SDL_Renderer* renderer);
void render_junction_box(SDL_Renderer* renderer);

#endif