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
#include "CollisionManager.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define NUM_POINTS 500
#define MIN_PIXELS_PER_SECOND 30  
#define MAX_PIXELS_PER_SECOND 60  

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static Uint64 last_time = 0;
static float point_speeds[NUM_POINTS];

Player* guy = nullptr;
UI* ui = nullptr;
Object* block = nullptr;
static std::vector<CharacterData> characters;
static std::vector<Sprite*> blocks;
static TextureCache* globalCache = nullptr;
std::vector<Sprite*> allCollidables;
std::vector<Sprite*> allObjects;
std::string gameMode = "PROD";
std::set<std::string> previousCollisions;

static CollisionManager colManager;
struct GameState {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TextureCache* cache = nullptr;
	std::vector<Sprite*> allObjects;
};
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (argv && argc > 0) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-d" || arg == "--debug") {
                std::cout << "Debug mode enabled!" << std::endl;
				gameMode = "DEBUG";
            }
        }
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) < 0) {
        return SDL_APP_FAILURE;
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_GAMEPAD);
    SDL_CreateWindowAndRenderer("BOY BOY BOY", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    globalCache = new TextureCache(renderer);
    loadTextures(globalCache);
    characters = loadCharacters(globalCache);
    float startX = WINDOW_WIDTH / 2;
    float startY = WINDOW_HEIGHT / 2;
   
    // Player 1
	Player* p1 = new Player(startX - 250, WINDOW_HEIGHT/2, characters[1], renderer);
	p1->playerID = 0;
	allObjects.push_back(p1);

	// Setup Player 2 (PvP!)
	//Player* p2 = new Player(450 + startX - 649 / 2 , WINDOW_HEIGHT / 2, characters[1], renderer);
	//std::cout << "Player 2 initilized as:" << characters[1].name << std::endl;
	//std::cout << "Player 2 width:" << characters[1].originalw << std::endl;
	//p2->playerID = 1;
	//p2->flipMode = SDL_FLIP_HORIZONTAL; // Face the other player
	//allObjects.push_back(p2);

	// UI elements
	blocks = loadObjects(globalCache, renderer);

	//get rid of boy boy boy for now
	//ui = new UI(startX - 689 /2, 0, globalCache, renderer);
	//allObjects.push_back(ui);

	allObjects.insert(allObjects.end(), blocks.begin(), blocks.end());
	last_time = SDL_GetTicks();

	for (auto obj : allObjects) {
		if (obj->canCollide) allCollidables.push_back(obj);
	}

	if (gameMode == "DEBUG") {
		for (auto object : allObjects) {
			object->debugMode = true;
		}
	}
	// Inside SDL_AppInit, right before returning CONTINUE
	for (auto obj : allObjects) obj->globalCacheRef = globalCache;
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
	for (auto obj : allObjects) {
		obj->handleInput(keys, elapsed);
		obj->update(elapsed);
	}

	// 3. Collision Detection
	colManager.update(allCollidables, previousCollisions);    

	// 4. Render
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	for (auto obj : allObjects) {
		obj->draw();
	}

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	for (auto obj : allObjects) {
		delete obj;
	}
	allObjects.clear();
	allCollidables.clear();

	globalCache->clear();
	delete globalCache; // Clean up the cache

	SDL_Quit();
}