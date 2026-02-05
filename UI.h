#pragma once
#pragma once
#include "Sprite.h"
#include "TextureCache.h"
#include <iostream>
class UI : public Sprite
{
public:
    bool redSwitch = true;
	bool blueSwitch = false;
	bool greenSwitch = false;
    UI(float x, float y, TextureCache* texCache, SDL_Renderer* renderer)
        : Sprite(x, y, 689, 145, 689, 145, renderer)
    {
        currentState = SpriteState::IDLE; 
        frameCounts[SpriteState::IDLE] = 1;
        animations[SpriteState::IDLE] = texCache->get("UI");

		this->modR = 255;
		this->modG = 0;
		this->modB = 0;
        
    }
    void update(float elapsed) {
        float speed = 1000.0f;

        if (modR == 255 && modG < 255 && modB == 0) modG = (Uint8)SDL_min(modG + (speed * elapsed), 255);
        else if (modG == 255 && modR > 0)             modR = (Uint8)SDL_max(modR - (speed * elapsed), 0);
        else if (modG == 255 && modB < 255)           modB = (Uint8)SDL_min(modB + (speed * elapsed), 255);
        else if (modB == 255 && modG > 0)             modG = (Uint8)SDL_max(modG - (speed * elapsed), 0);
        else if (modB == 255 && modR < 255)           modR = (Uint8)SDL_min(modR + (speed * elapsed), 255);
        else if (modR == 255 && modB > 0)             modB = (Uint8)SDL_max(modB - (speed * elapsed), 0);
	}
};