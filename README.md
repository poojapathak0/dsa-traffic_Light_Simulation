# Traffic Junction Simulator
  ![Image](https://github.com/user-attachments/assets/37c63a52-4618-4f27-8b67-04561709a4d4)
  
A real-time traffic junction simulation system that models vehicle flow, traffic light cycles, and priority vehicle handling at a four-way intersection.

## Overview

This project simulates a four-way traffic junction with traffic lights controlling vehicle flow from four directions (labeled A, B, C, and D). The simulation includes:

- Visualized traffic flow with SDL graphics
- Multiple vehicle types with different priorities (Normal, VIP, Ambulance)
- Synchronized traffic light system
- Real-time vehicle generation and queue management
- Multi-threaded architecture for concurrent operations

## Components

### Core Modules

1. **Main Simulation Engine** (`main.c`)
   - Initializes SDL graphics system
   - Manages rendering loop
   - Coordinates data between threads

2. **Vehicle Management** (`vehicle.h`, `vehicle.c`)
   - Defines vehicle properties and behaviors
   - Handles vehicle movement logic
   - Renders vehicles with distinct visual indicators

3. **Traffic Light System** (`traffic_light.h`, `traffic_light.c`)
   - Controls traffic light cycles
   - Renders traffic light visuals
   - Manages state transitions

4. **Queue Systems** (`queue.h`, `queue.c`, `priority_queue.h`, `priority_queue.c`)
   - Standard queue implementation for basic vehicle management
   - Priority queue implementation for handling emergency vehicles

5. **Vehicle Generator** (`vehicle_generator.c`)
   - Creates random vehicles with varied properties
   - Writes vehicle data to a file for the simulator to process

### Data Structures

- **Vehicle**: Represents a vehicle with ID, lane, type, position, and movement state
- **TrafficLight**: Manages light states, position, and rendering
- **Queue/PriorityQueue**: Data structures for managing vehicle flow

## Getting Started

### Prerequisites

- SDL2 library
- C compiler (compatible with C99 or later)
- Windows OS (for Windows-specific functions)
- Make (for building the project)

### Building the Project

The project uses a Makefile for compilation:

```
make
```

This will compile all the necessary files and create the executables in the `bin` directory.

### Running the Simulation

1. Start the vehicle generator:
   ```
   ./bin/generator.exe
   ```

2. In a separate terminal, run the simulator:
   ```
   ./bin/simulator.exe
   ```

## How It Works

### Traffic Flow

- Vehicles are generated from four directions (A, B, C, D)
- Each direction has its own queue
- Traffic lights alternate between two states:
  - State 1: A and C directions have green lights
  - State 2: B and D directions have green lights

### Vehicle Types

- **Normal**: Regular vehicles with standard priority
- **VIP**: Higher priority vehicles that are processed first from the queue
- **Ambulance**: Emergency vehicles that can pass through intersections regardless of traffic light state

### Threads

The simulation uses three main threads:
1. **Main Thread**: Handles rendering and user interaction
2. **Queue Processing Thread**: Manages traffic light cycles and vehicle movement
3. **Data Reading Thread**: Reads vehicle data from the external file

## File Format

The vehicle generator creates entries in `vehicles.data` with the format:
```
[VehicleID]:[Lane]:[Type]:[Direction]
```

Example:
```
AB1CD234:A:NORMAL:STRAIGHT
EF2GH567:B:AMBULANCE:LEFT
```

## Configuration

Several parameters can be adjusted in the code:
- `WINDOW_WIDTH` and `WINDOW_HEIGHT`: Simulation window dimensions
- `MAX_VEHICLES`: Maximum number of active vehicles
- Traffic light timing cycles
- Vehicle speeds and dimensions

## Project Structure

```
project/
│
├── src/                # Source files
│   ├── main.c
│   ├── vehicle.c
│   ├── traffic_light.c
│   ├── queue.c
│   ├── priority_queue.c
│   └── vehicle_generator.c
│
├── include/            # Header files
│   ├── vehicle.h
│   ├── traffic_light.h
│   ├── queue.h
│   ├── priority_queue.h
│   └── traffic_system.h
│
├── bin/                # Compiled executables
│   ├── simulator.exe   # Main simulation program
│   └── generator.exe   # Vehicle generator program
│
├── Makefile            # Build instructions
│
└── vehicles.data       # Generated vehicle data
```

## Future Improvements

- Add more sophisticated traffic patterns
- Implement turn signals and lane changes
- Add pedestrian crossings
- Create a configuration interface
- Add statistics and performance metrics
