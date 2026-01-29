#pragma once
#include "Sprite.h"
#include "TextureManager.h"
class WoodCutter : public Sprite {
public:
    WoodCutter(float x, float y, TextureManager* texManager, SDL_Renderer* renderer)
        : Sprite(x, y, 96, 96, texManager, renderer,  "Sprites/1 Woodcutter/Woodcutter.png", "Wood Cutter", "Sprites/1 Woodcutter/Woodcutter_walk.png")
    {
    }
};


