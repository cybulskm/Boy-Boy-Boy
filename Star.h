#pragma once
#include "Sprite.h"
class Star : public Sprite {
public:
    Star(float x, float y, float w, float h, SDL_Renderer* renderer)
        : Sprite(x, y, w, h, renderer, "Sprites/1 Woodcutter/Woodcutter.png", "Star")
    {
    }
};
