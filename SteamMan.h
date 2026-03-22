#pragma once
#include "TextureCache.h"
#include "CharacterData.h"

inline CharacterData createSteamMan(TextureCache* texCache) {
    CharacterData data;
    data.name = "Steam Man";

    // Keys for GIF logic
    data.animKeys[SpriteState::IDLE] = "WOODMAN_IDLE";
    data.animKeys[SpriteState::WALKING] = "WOODMAN_WALKING";
    data.animKeys[SpriteState::JUMPING] = "WOODMAN_JUMPING";
    data.animKeys[SpriteState::ATTACK1] = "WOODMAN_ATTACK1";
    data.animKeys[SpriteState::ATTACK2] = "WOODMAN_ATTACK2";

    // Legacy texture pointers
    data.animations[SpriteState::IDLE] = texCache->get("WOODMAN_IDLE");
    data.animations[SpriteState::WALKING] = texCache->get("WOODMAN_WALKING");
    data.animations[SpriteState::JUMPING] = texCache->get("WOODMAN_JUMPING");
    data.animations[SpriteState::ATTACK1] = texCache->get("WOODMAN_ATTACK1");
    data.animations[SpriteState::ATTACK2] = texCache->get("WOODMAN_ATTACK2");

    // Frame counts (Auto-detect if GIF)
    data.frameCounts[SpriteState::IDLE] = texCache->getCount("WOODMAN_IDLE", 1);
    data.frameCounts[SpriteState::WALKING] = texCache->getCount("WOODMAN_WALKING", 6);
    data.frameCounts[SpriteState::JUMPING] = texCache->getCount("WOODMAN_JUMPING", 6);
    data.frameCounts[SpriteState::ATTACK1] = texCache->getCount("WOODMAN_ATTACK1", 6);
    data.frameCounts[SpriteState::ATTACK2] = texCache->getCount("WOODMAN_ATTACK2", 6);
    
    data.originalw = 48.0f;
    data.originalh = 48.0f;
    data.draww = 96.0f;
    data.drawh = 96.0f;
    data.velocityX = 300.0f;
    data.health = 150.0f;
    data.heavyAttackCoolDown = 5;

    return data;
}