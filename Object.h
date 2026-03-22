#pragma once
#include "Sprite.h"
#include "TextureCache.h"
#include "SpriteState.h"
class Object : public Sprite {
public:
    Object(float x, float y, TextureCache* texCache, SDL_Renderer* renderer)
        : Sprite(x, y, 223, 32, 800, 50, renderer)
    {
        currentState = SpriteState::IDLE;
        frameCounts[SpriteState::IDLE] = 1;
        animations[SpriteState::IDLE] = texCache->get("BLOCK");
        this->name = "BLOCK";
        this->canCollide = true;

        // Hitbox matches the visual rectangle exactly
        this->hitboxOffsetX = 0;
        this->hitboxOffsetY = 0;
        this->hitboxW = 800;
        this->hitboxH = 50;
    }
};