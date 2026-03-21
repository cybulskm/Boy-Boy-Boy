#pragma once
#include "TextureCache.h"
#include "SteamMan.h"
#include "Knight.h"
#include <vector>
#include "SDL3/SDL.h"
#include "CharacterData.h"
#include <iostream>
#include <string>

std::string base_path = "Sprites";
std::string steam_man_path = base_path + "/3 SteamMan";
std::string knight_path = base_path + "/GifSet1/NoOutline";
std::string knight_path_gif = knight_path + "/120x80_gifs";
std::string knight_path_spritesheet = knight_path + "/120x80_PNGSheets";

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


    //STATIC OBJS
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
    Knight knight(globalCache);
    characters.push_back(knight.data);

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

