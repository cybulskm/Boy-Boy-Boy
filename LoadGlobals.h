#pragma once
#include "TextureCache.h"
#include "SteamMan.h"
#include <vector>
#include "SDL3/SDL.h"
#include "CharacterData.h"

void loadTextures(TextureCache* globalCache) {
    globalCache->load("WOODMAN_IDLE", "Sprites/3 SteamMan/SteamMan.png");
    globalCache->load("WOODMAN_WALKING", "Sprites/3 SteamMan/SteamMan_walk.png");
    globalCache->load("WOODMAN_JUMPING", "Sprites/3 SteamMan/SteamMan_jump.png");
    globalCache->load("WOODMAN_ATTACK1", "Sprites/3 SteamMan/SteamMan_attack1.png");
    globalCache->load("WOODMAN_ATTACK2", "Sprites/3 SteamMan/SteamMan_attack2.png");
    globalCache->load("BLOCK", "Sprites/4 Effects/board.png");
    globalCache->load("UI", "Sprites/TITLE - Copy.png");
}

void unloadTextures(TextureCache* globalCache) {
    globalCache->clear();
}

static std::vector<CharacterData> loadCharacters(TextureCache* globalCache) {
    static std::vector<CharacterData> characters = {};
    SteamMan steamMan(globalCache);
    characters.push_back(steamMan.data);

	return characters;
}

void loadEnviornment() {
        // Placeholder for future environment loading logic
}

void loadObjects() {}

void loadPlayers() {}