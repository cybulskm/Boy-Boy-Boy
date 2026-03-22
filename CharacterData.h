#pragma once
#include <map>
#include <string>
#include <SDL3/SDL.h>
#include "SpriteState.h"
#include "string.h"
struct CharacterData {
    std::map<SpriteState, SDL_Texture*> animations; // RESTORED
    std::map<SpriteState, std::string> animKeys;    // ADDED FOR GIFS
    std::map<SpriteState, int> frameCounts;
    float originalw = 0;
    float originalh = 0;
    float draww = 0;
    float drawh = 0;
    float velocityX = 0;
    float health = 0;
    int heavyAttackCoolDown = 0;
    std::string name;
    float hitboxW = 0;
    float hitboxH = 0;
    float offsetX = 0;
    float offsetY = 0;
    float attackHitboxIncrease = 0;
    CharacterData() {};
    //for blocks
    CharacterData(float originalw, float originalh, float draww, float drawh, std::string name) {
        this->originalw = originalw;
        this->originalh = originalh;
        this->draww = draww;
        this->drawh = drawh;
        this->name = name;
        // Hitbox matches the visual rectangle exactly
        this->offsetX = 0;
        this->offsetY = 0;
        this->hitboxW = draww;
        this->hitboxH = drawh;
    }
};