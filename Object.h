#pragma once
#include "Sprite.h"
#include "TextureCache.h"
#include "SpriteState.h"
class Object : public Sprite {
public:
    Object(float x, float y, const CharacterData& charData, SDL_Renderer* renderer)
        : Sprite(x, y, charData, renderer)
    {
        currentState = SpriteState::IDLE;
        frameCounts[SpriteState::IDLE] = 1;
        this->name = "BLOCK";
        this->canCollide = true;
    }
    Object(float x, float y, float originalw, float originalh, float draww, float drawh, SDL_Renderer* renderer)
        : Sprite(x, y, originalw, originalh, draww, drawh, renderer)
    {
        currentState = SpriteState::IDLE;
        frameCounts[SpriteState::IDLE] = 1;
        this->name = "BLOCK";
        this->canCollide = true;
        this->hitboxOffsetX = 0;
        this->hitboxOffsetY = 0;
        this->hitboxW = draww;
        this->hitboxH = drawh;
    }

};