#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <SDL.h>

typedef struct {
    int x, y;
    int state; // 0=red, 1=green
} TrafficLight;

void traffic_light_init(TrafficLight* lights);
void traffic_light_update(TrafficLight* lights, int currentState);
void traffic_light_draw(SDL_Renderer* renderer, TrafficLight* light);

#endif // TRAFFIC_LIGHT_H