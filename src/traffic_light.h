#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <SDL.h>

#define GREEN 1
#define YELLOW 2
#define RED 0

typedef struct {
    int x, y;
    int state;
} TrafficLight;

void traffic_light_init(TrafficLight* lights);
void traffic_light_update(TrafficLight* lights, int currentState);
void traffic_light_draw(SDL_Renderer* renderer, TrafficLight* light);

#endif // TRAFFIC_LIGHT_H