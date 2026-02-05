#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <iostream>
#include "Sprite.h"
#include "TextureCache.h"   
#include "Player.h"
#include "UI.h"
#include "SteamMan.h"
#include "LoadGlobals.h"
#include "Object.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define NUM_POINTS 500
#define MIN_PIXELS_PER_SECOND 30  
#define MAX_PIXELS_PER_SECOND 60  

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static Uint64 last_time = 0;
static float point_speeds[NUM_POINTS];
static std::vector<Player*> entities;
static std::vector<UI*> ui_elements;
static std::vector<Object*> objects;
Player* guy = nullptr;
UI* ui = nullptr;
Object* block = nullptr;
static std::vector<CharacterData> characters;
static TextureCache* globalCache = nullptr;

std::vector<Sprite*> allCollidables;


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    std::cout << "The program is looking here: " << SDL_GetBasePath() << std::endl;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_GAMEPAD);
    SDL_CreateWindowAndRenderer("BOY BOY BOY", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    globalCache = new TextureCache(renderer);
    loadTextures(globalCache);
    characters = loadCharacters(globalCache);
    float startX = WINDOW_WIDTH / 2;
    float startY = WINDOW_HEIGHT / 2;
    Player* p1 = new Player(400, 500, characters[0], renderer);
    p1->playerID = 0;
    entities.push_back(p1);

    // Setup Player 2 (PvP!)
    Player* p2 = new Player(1200, 500, characters[0], renderer);
    p2->playerID = 1;
    p2->flipMode = SDL_FLIP_HORIZONTAL; // Face the other player
    entities.push_back(p2);
    ui = new UI(startX, 0, globalCache, renderer);
	block = new Object(startX - 250, startY + 50, globalCache, renderer);

    ui_elements.push_back(ui);
	objects.push_back(block);
    last_time = SDL_GetTicks();
    for (auto o : objects) allCollidables.push_back(o);
    for (auto e : entities) allCollidables.push_back(e);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    // 1. Time & Input
    const float elapsed = (float)(SDL_GetTicks() - last_time) / 1000.0f;
    last_time = SDL_GetTicks();
    const bool* keys = SDL_GetKeyboardState(NULL);

    // 2. Logic (Update all first)
    for (auto entity : entities) {
        entity->handleInput(keys, elapsed);
    }
    for (auto e : entities) e->update(elapsed);

    // 3. Render
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    for (auto obj : objects) obj->draw();
    for (auto e : entities) e->draw();
    for (auto ui : ui_elements) ui->draw();

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    for (auto entity : entities) delete entity;
    for (auto ui_e : ui_elements) delete ui_e;
	for (auto obj : objects) delete obj;
    globalCache->clear();
    delete globalCache; // Clean up the cache

    SDL_Quit();
}