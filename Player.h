#pragma once
#include "Sprite.h"
#include "TextureCache.h"
#include <iostream>
class Player : public Sprite {
public:
    bool isJumping = false;
    bool isMoving = false;
    float velocityY = 0.0f;
    float groundY;
    const float gravity = 1500.0f;
    static constexpr float DEFAULT_W = 48.0f;
    static constexpr float DEFAULT_H = 48.0f;
    Player(float x, float y, TextureCache* texCache, SDL_Renderer* renderer)
        : Sprite(x, y, DEFAULT_W, DEFAULT_H, DEFAULT_W, DEFAULT_H, renderer)
    {
        this->groundY = y;

        frameCounts[SpriteState::IDLE] = 1;
        frameCounts[SpriteState::WALKING] = 6;
        frameCounts[SpriteState::JUMPING] = 6;
        animations[SpriteState::IDLE] = texCache->get("WOODMAN_IDLE");
        animations[SpriteState::WALKING] = texCache->get("WOODMAN_WALKING");
        animations[SpriteState::JUMPING] = texCache->get("WOODMAN_JUMPING");
        
    }
    
    void handleInput(const bool* keys, float speed, float elapsed) {
        isMoving = false;
        if (keys[SDL_SCANCODE_LEFT]) {
            x -= speed * elapsed;
            flipMode = SDL_FLIP_HORIZONTAL;
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            x += speed * elapsed;
            flipMode = SDL_FLIP_NONE;
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_UP] && !isJumping) {
            velocityY = -600.0f;
            isJumping = true;
        }
        if (keys[SDL_SCANCODE_X]){
            isHurt();
		}
    }
    void isHurt() {
        
        setColour(255, 0, 0);
	}
    void update(float elapsed) {
        // Physics logic
        if (isJumping) {
            velocityY += gravity * elapsed;
            y += velocityY * elapsed;
            if (y >= groundY) {
                y = groundY;
                isJumping = false;
                velocityY = 0;
            }
        }

        // State machine logic
        SpriteState lastState = currentState;
        if (isJumping) currentState = SpriteState::JUMPING;
        else if (isMoving) currentState = SpriteState::WALKING;
        else currentState = SpriteState::IDLE;

        // Reset frame counter on state change for smooth transitions
        if (lastState != currentState) {
            currentFrame = 0;
            animationTimer = 0.0f;
        }

        // Sync drawing rect with physics x/y
        rect.x = x;
        rect.y = y;

        animate(elapsed);
    }
};