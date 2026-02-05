#pragma once
#include <map>
#include <SDL3/SDL.h>
#include "SpriteState.h"
struct CharacterData {
    std::map<SpriteState, SDL_Texture*> animations;
    float originalw, originalh, draww, drawh;
    float velocityX;
    float health;
    int lightAttackCoolDown;
    int heavyAttackCoolDown;
    std::map<SpriteState, int> frameCounts;
};