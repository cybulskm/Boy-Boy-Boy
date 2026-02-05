#pragma once
#include "Sprite.h"
#include "TextureCache.h"
#include <iostream>
#include "SteamMan.h"
#include "SpriteState.h"
class Player : public Sprite {
public:
    int playerID;
    bool isJumping = false;
    bool isMoving = false;
    bool isLightAttacking = false;
	bool isHeavyAttacking = false;
    float velocityY = 0.0f;
	float velocityX = 300.0f;
    float groundY;
    const float gravity = 1500.0f;
    float health = 0.0f;
    const std::string currentCharacter = "STEAMMAN";
    int heavyAttackCoolDown = 0;
    Player(float x, float y, const CharacterData& charData, SDL_Renderer* renderer)
        : Sprite(x, y, charData.originalw, charData.originalh, charData.draww, charData.drawh, renderer)
    {
        this->groundY = y;

        // Copy the attributes from the character data to the player
        this->animations = charData.animations;
        this->velocityX = charData.velocityX;
        this->health = charData.health;
        this->heavyAttackCoolDown = charData.heavyAttackCoolDown;
		this->frameCounts = charData.frameCounts;
		this->currentState = SpriteState::IDLE;
    }

    void handleInput(const bool* keys, float elapsed) {
        isMoving = false;

        // Define controls based on Player ID
        SDL_Scancode up = (playerID == 0) ? SDL_SCANCODE_W : SDL_SCANCODE_UP;
        SDL_Scancode left = (playerID == 0) ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT;
        SDL_Scancode right = (playerID == 0) ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT;
        SDL_Scancode atk1 = (playerID == 0) ? SDL_SCANCODE_1 : SDL_SCANCODE_N;
        SDL_Scancode atk2 = (playerID == 0) ? SDL_SCANCODE_2 : SDL_SCANCODE_M;

        if (keys[left]) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; isMoving = true; }
        if (keys[right]) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; isMoving = true; }
        if (keys[up] && !isJumping) { velocityY = -700.0f; isJumping = true; }
        

        // Attacks
        if (keys[atk1] && !isLightAttacking && !isHeavyAttacking) lightAttack();
        if (keys[atk2] && !isHeavyAttacking && !isLightAttacking) heavyAttack();
    }

    void lightAttack() {
        isLightAttacking = true;
        currentState = SpriteState::ATTACK1;
        currentFrame = 0;
        animationTimer = 0.0f;
    }

    void heavyAttack() {
        isHeavyAttacking = true;
        currentState = SpriteState::ATTACK2;
        currentFrame = 0;
        animationTimer = 0.0f;
    }

    void update(float elapsed) {
        // 1. Always run Physics (Movement and Jump work regardless of animation)
        if (isJumping) {
            velocityY += gravity * elapsed;
            y += velocityY * elapsed;
            if (y >= groundY) {
                y = groundY;
                isJumping = false;
            }
        }

        // 2. Animation State Priority (The "Visual" Layer)
        SpriteState lastState = currentState;

        if (isLightAttacking) {
            currentState = SpriteState::ATTACK1;
            if (currentFrame >= frameCounts[currentState] - 1) {
                isLightAttacking = false;
            }
        }
        else if (isHeavyAttacking) {
            currentState = SpriteState::ATTACK2;
            if (currentFrame >= frameCounts[currentState] - 1) {
                isHeavyAttacking = false;
            }
		}
        else if (isJumping) {
            currentState = SpriteState::JUMPING;
        }
        else if (isMoving) {
            currentState = SpriteState::WALKING;
        }
        else {
            currentState = SpriteState::IDLE;
        }

        // Reset timer only if the state actually changed
        if (lastState != currentState) {
            currentFrame = 0;
            animationTimer = 0.0f;
        }

        rect.x = x;
        rect.y = y;
        animate(elapsed);
    }

    void isHurt() {
        setColour(255, 0, 0);
    }
};