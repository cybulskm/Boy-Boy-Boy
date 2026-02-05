#pragma once
#include "Sprite.h"
#include "TextureCache.h"
#include "SpriteState.h"
class Object : public Sprite {
public:
    Object(float x, float y, TextureCache* texCache, SDL_Renderer* renderer)
        : Sprite(x, y, 223, 32, 500, 50, renderer)
    {
        currentState = SpriteState::IDLE;
        frameCounts[SpriteState::IDLE] = 1;
        animations[SpriteState::IDLE] = texCache->get("BLOCK");
    }
};