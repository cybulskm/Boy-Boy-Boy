#pragma once
#include "TextureCache.h"
#include "SteamMan.h"
#include "Knight.h"
#include <vector>
#include "SDL3/SDL.h"
#include "CharacterData.h"
#include <iostream>
#include <string>
#include "Object.h"

std::string base_path = "Sprites";
std::string steam_man_path = base_path + "/3 SteamMan";
std::string knight_path = base_path + "/Knight";
std::string knight_path_gif = knight_path + "/gifs";
std::string knight_path_spritesheet = knight_path + "/spritesheets";

void loadTextures(TextureCache* globalCache) {
    //STEAM MAN
    globalCache->load("WOODMAN_IDLE", "Sprites/3 SteamMan/SteamMan.png");
    globalCache->load("WOODMAN_WALKING", "Sprites/3 SteamMan/SteamMan_walk.png");
    globalCache->load("WOODMAN_JUMPING", "Sprites/3 SteamMan/SteamMan_jump.png");
    globalCache->load("WOODMAN_ATTACK1", "Sprites/3 SteamMan/SteamMan_attack1.png");
    globalCache->load("WOODMAN_ATTACK2", "Sprites/3 SteamMan/SteamMan_attack2.png");
    globalCache->load("WOODMAN_ATTACK2", "Sprites/GifSet1/NoOutline/120x80_PNGSheets/_Attack.png");
    //KNIGHT
    globalCache->load("KNIGHT_IDLE", knight_path_gif + "/__Idle.gif");
    globalCache->load("KNIGHT_RUNNING", knight_path_gif + "/__Run.gif");
    globalCache->load("KNIGHT_JUMPING", knight_path_gif + "/__Jump.gif");
    globalCache->load("KNIGHT_ATTACK1", knight_path_gif + "/__Attack.gif");
    globalCache->load("KNIGHT_ATTACK2", knight_path_gif + "/__Attack2.gif");
    globalCache->load("KNIGHT_CROUCHING", knight_path_gif + "/__Crouch.gif");
    globalCache->load("KNIGHT_ROLLING", knight_path_gif + "/__Roll.gif");
    globalCache->load("KNIGHT_DEATH", knight_path_gif + "/__Death.gif");
    globalCache->load("KNIGHT_SLIDING", knight_path_gif + "/__Slide.gif");
    globalCache->load("KNIGHT_HURT", knight_path_gif + "/__Hit.gif");


    //STATIC OBJS
    globalCache->load("BLOCK", "Sprites/4 Effects/board.png");
    globalCache->load("UI", "Sprites/TITLE - Copy.png");

}

void unloadTextures(TextureCache* globalCache) {
    globalCache->clear();
}

static std::vector<CharacterData> loadCharacters(TextureCache* globalCache) {
    static std::vector<CharacterData> characters = {};
    characters.push_back(createSteamMan(globalCache));
    characters.push_back(createKnight(globalCache));
	return characters;
}

void loadEnviornment() {
        // Placeholder for future environment loading logic
}

static std::vector<Sprite*> loadObjects(TextureCache* globalCache, SDL_Renderer* renderer) {
    std::vector<Sprite*> allObjects;
    struct ObjectPos { float x, y, w, h; };
    std::vector<ObjectPos> positions = { {400, 836, 800, 64}, {0, 400, 223, 32} };

    for (const auto& pos : positions) {
        Object* obj = new Object(pos.x, pos.y, 223, 32, pos.w, pos.h, renderer);
        obj->animations[SpriteState::IDLE] = globalCache->get("BLOCK");;
        allObjects.push_back(obj);
    }
    return allObjects;
}

void loadPlayers() {}

