#pragma once
#include "TextureCache.h"
#include "SpriteState.h"
#include "CharacterData.h"

inline CharacterData createKnight(TextureCache* texCache) {
    CharacterData data;
    // Keys for GIF logic
    data.animKeys[SpriteState::IDLE] = "KNIGHT_IDLE";
    data.animKeys[SpriteState::WALKING] = "KNIGHT_RUNNING";
    data.animKeys[SpriteState::JUMPING] = "KNIGHT_JUMPING";
    data.animKeys[SpriteState::ATTACK1] = "KNIGHT_ATTACK1";
    data.animKeys[SpriteState::ATTACK2] = "KNIGHT_ATTACK2";
    data.animKeys[SpriteState::CROUCH] = "KNIGHT_CROUCHING";
    data.animKeys[SpriteState::ROLLING] = "KNIGHT_ROLLING";
    data.animKeys[SpriteState::SLIDING] = "KNIGHT_SLIDING";
    data.animKeys[SpriteState::HURT] = "KNIGHT_HURT";


    // Frame counts (Auto-detect if GIF)
    data.frameCounts[SpriteState::IDLE] = texCache->getCount("KNIGHT_IDLE", 0);
    data.frameCounts[SpriteState::WALKING] = texCache->getCount("KNIGHT_RUNNING", 0);
    data.frameCounts[SpriteState::JUMPING] = texCache->getCount("KNIGHT_JUMPING", 0);
    data.frameCounts[SpriteState::ATTACK1] = texCache->getCount("KNIGHT_ATTACK1", 0);
    data.frameCounts[SpriteState::ATTACK2] = texCache->getCount("KNIGHT_ATTACK2", 0);
    data.frameCounts[SpriteState::CROUCH] = texCache->getCount("KNIGHT_CROUCHING", 0);
    data.frameCounts[SpriteState::ROLLING] = texCache->getCount("KNIGHT_ROLLING", 0);
    data.frameCounts[SpriteState::SLIDING] = texCache->getCount("KNIGHT_SLIDING", 0);
    data.frameCounts[SpriteState::HURT] = texCache->getCount("KNIGHT_SLIDING", 0);

    data.originalw = 120.0f;
    data.originalh = 80.0f;
    data.draww = 240.0f;
    data.drawh = 160.0f;
    data.velocityX = 400.0f;
    data.health = 150.0f;
    data.heavyAttackCoolDown = 5;
    data.name = "Knight";
    // CUSTOM HITBOX (This won't be overwritten because it's in the data struct)
    data.hitboxW = 50.0f;   // The Knight is thin
    data.hitboxH = 120.0f;  // He's tall
    // Center the box horizontally: (TotalWidth - HitboxWidth) / 2
    data.offsetX = (data.draww - data.hitboxW) / 2.0f;
    // Put the box at the feet: (TotalHeight - HitboxHeight)
    data.offsetY = data.drawh - data.hitboxH;
    data.attackHitboxIncrease = 65.0f;
    return data;
};