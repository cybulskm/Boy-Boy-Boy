#pragma once
#include "TextureCache.h"
#include "SteamMan.h"
#include <vector>
#include "SDL3/SDL.h"
#include "CharacterData.h"
#include <iostream>
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

void handleCollision(Sprite* a, Sprite* b) {
    // Simple response: If one is an attack and the other is a player, hurt the player
    //if (a->velocityX >= 0 && b->velocityX <= 0) {
        //a->velocityX = 0;
		//b->velocityX = 0;
	//}
	std::cout << a->name << " collided with " << b->name << std::endl;
    if ((a->currentState == SpriteState::ATTACK1 || a->currentState == SpriteState::ATTACK2) && dynamic_cast<Player*>(b)) {
        dynamic_cast<Player*>(b)->isHurt;
    }
    else if ((b->currentState == SpriteState::ATTACK1 || b->currentState == SpriteState::ATTACK2) && dynamic_cast<Player*>(a)) {
        dynamic_cast<Player*>(a)->isHurt;
    }
}

void resolveCollisions(std::vector<Sprite*>& sprites) {
    for (size_t i = 0; i < sprites.size(); ++i) {
        for (size_t j = i + 1; j < sprites.size(); ++j) {
            Sprite* a = sprites[i];
            Sprite* b = sprites[j];

            // Only check if BOTH intended to collide
            if (a->canCollide && b->canCollide) {
                if (a->intersects(a->getBounds(), b->getBounds())) {
                    // Trigger a response
                    handleCollision(a, b);
                }
            }
        }
    }
}

