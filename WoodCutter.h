#pragma once
#include "Sprite.h"
class WoodCutter : public Sprite {
public:
    WoodCutter(float x, float y, SDL_Renderer* renderer)
        : Sprite(x, y, 48, 48, renderer, "Sprites/1 Woodcutter/Woodcutter.png", "Wood Cutter")
    {
    }
};


