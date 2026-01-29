#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>
#include "TextureManager.h"
class AnimationManager {
	public:
	static void animate_walking(float& animationTimer, int& currentFrame, const int totalFrames, float frameDuration, bool isMoving, SDL_FRect& srcRect, float elapsed) {
		if (!isMoving) {
			currentFrame = 0; 
			srcRect.x = 0;
			return;
		}
		animationTimer += elapsed;
		if (animationTimer >= frameDuration) {
			animationTimer = 0.0f;
			currentFrame = (currentFrame + 1) % totalFrames;
			srcRect.x = currentFrame * srcRect.w;
		}
	}
};