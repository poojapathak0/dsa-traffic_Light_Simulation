#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <SDL.h>
#include <stdbool.h>

typedef struct {
    char direction;
    SDL_Point position;
    bool is_red;
} TrafficLight;

void traffic_light_init(TrafficLight* lights);
void traffic_light_draw(SDL_Renderer* renderer, TrafficLight* light);
void traffic_light_update(TrafficLight* lights, int state);

#endif