#pragma once

#include "TextureCache.h"
#include <iostream>
#include "SpriteState.h"
#include "CharacterData.h"
class SteamMan
{
public:
    CharacterData data;
    SteamMan(TextureCache* texCache) {
        data.animations[SpriteState::IDLE] = texCache->get("WOODMAN_IDLE");
        data.animations[SpriteState::WALKING] = texCache->get("WOODMAN_WALKING");
        data.animations[SpriteState::JUMPING] = texCache->get("WOODMAN_JUMPING");
        data.animations[SpriteState::ATTACK1] = texCache->get("WOODMAN_ATTACK1");
        data.animations[SpriteState::ATTACK2] = texCache->get("WOODMAN_ATTACK2");
        
        data.originalw = 48.0f;
        data.originalh = 48.0f;
        data.draww = 48.0f;
        data.drawh = 48.0f;
        data.velocityX = 500.0f;
        data.health = 150.0f;
        data.heavyAttackCoolDown = 5;
        data.frameCounts[SpriteState::IDLE] = 1;
        data.frameCounts[SpriteState::WALKING] = 6;
        data.frameCounts[SpriteState::JUMPING] = 6;
        data.frameCounts[SpriteState::ATTACK1] = 6;
        data.frameCounts[SpriteState::ATTACK2] = 6;
    }
       
};