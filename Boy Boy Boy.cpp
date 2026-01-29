#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <iostream>
#include "Sprite.h"
#include "WoodCutter.h"
#include "TextureManager.h" 

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define NUM_POINTS 500
#define MIN_PIXELS_PER_SECOND 30  
#define MAX_PIXELS_PER_SECOND 60  

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static Uint64 last_time = 0;
static float point_speeds[NUM_POINTS];
static std::vector<Sprite*> entities;
float velocityx = 1.0f;
WoodCutter* guy = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("BOY BOY BOY", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    /*LOAD CHARACTER POSITIONS HERE*/
    
    float startX = WINDOW_WIDTH / 2;
    float startY = WINDOW_HEIGHT / 2;
    TextureManager* texManager = new TextureManager(renderer);
    for (int i = 0; i <= 1000; i++) {
		startX = rand() % (WINDOW_WIDTH - 100);
		startY = rand() % (WINDOW_HEIGHT - 100);
        guy = new WoodCutter(startX, startY, texManager, renderer);
        /*For every entitnity, add it to the list*/
        entities.push_back(guy);
    }
    

    last_time = SDL_GetTicks();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    // 1. Calculate Delta Time
    const Uint64 now = SDL_GetTicks();
    const float elapsed = (float)(now - last_time) / 1000.0f;
    last_time = now;

    const bool* keys = SDL_GetKeyboardState(NULL);
    float speed = 300.0f; 
    for (auto entity : entities) {
        speed -= 0.1f;
		entity->handleInput(keys, speed, elapsed);
    }
    //guy->handleInput(keys, speed, elapsed);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (auto entity : entities) {
        entity->update(elapsed);
        entity->draw();
    }

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    for (auto entity : entities) {
        delete entity;
    }
    entities.clear();
    SDL_Quit();
}