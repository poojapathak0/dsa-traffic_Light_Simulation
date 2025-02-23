#include "traffic_light.h"

void traffic_light_init(TrafficLight* lights) {
    lights[0] = (TrafficLight){'A', {400, 300}, true};
    lights[1] = (TrafficLight){'B', {400, 500}, true};
    lights[2] = (TrafficLight){'C', {500, 400}, true};
    lights[3] = (TrafficLight){'D', {300, 400}, true};
}

void traffic_light_draw(SDL_Renderer* renderer, TrafficLight* light) {
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_Rect lightBox = {light->position.x, light->position.y, 50, 30};
    SDL_RenderFillRect(renderer, &lightBox);

    if(light->is_red) 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else 
        SDL_SetRenderDrawColor(renderer, 11, 156, 50, 255);

    SDL_Rect lightCircle = {light->position.x + 5, light->position.y + 5, 20, 20};
    SDL_RenderFillRect(renderer, &lightCircle);
}

void traffic_light_update(TrafficLight* lights, int state) {
    switch(state) {
        case 0:
            for(int i = 0; i < 4; i++) lights[i].is_red = true;
            break;
        case 1:
            lights[0].is_red = false;
            lights[1].is_red = true;
            lights[2].is_red = false;
            lights[3].is_red = true;
            break;
        case 2:
            lights[0].is_red = true;
            lights[1].is_red = false;
            lights[2].is_red = true;
            lights[3].is_red = false;
            break;
    }
}
