#pragma once
#include "Sprite.h"
#include <iostream>
#include "SpriteState.h"

class Player : public Sprite {
public:
    int playerID = 0;
    bool isJumping = false;
    bool isMoving = false;
    bool isCrouching = false;
    bool isSliding = false;
    bool isLightAttacking = false;
    bool isHeavyAttacking = false;
    bool isRolling = false;
    bool isDead = false;
    bool isHurt = false;

    float groundY;
    const float gravity = 1500.0f;
    float slideTimer = 0.0f;
    const float slideDuration = 0.5f;

    float slideCooldownTimer = 0.0f;
    const float slideCooldownDuration = 1.2f;

    Player(float x, float y, const CharacterData& charData, SDL_Renderer* renderer)
        : Sprite(x, y, charData, renderer)
    {
        this->name = charData.name;
        this->groundY = y;
        this->currentState = SpriteState::IDLE;
    }

    void handleInput(const bool* keys, float elapsed) {
        isMoving = false;

        SDL_Scancode up = (playerID == 0) ? SDL_SCANCODE_W : SDL_SCANCODE_UP;
        SDL_Scancode left = (playerID == 0) ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT;
        SDL_Scancode right = (playerID == 0) ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT;
        SDL_Scancode down = (playerID == 0) ? SDL_SCANCODE_S : SDL_SCANCODE_DOWN;
        SDL_Scancode atk1 = (playerID == 0) ? SDL_SCANCODE_1 : SDL_SCANCODE_N;
        SDL_Scancode atk2 = (playerID == 0) ? SDL_SCANCODE_2 : SDL_SCANCODE_M;

        if (slideCooldownTimer > 0) slideCooldownTimer -= elapsed;

        if (keys[down] && !isJumping && !isRolling) {
            if ((keys[left] || keys[right]) && !isSliding && slideCooldownTimer <= 0) {
                isSliding = true;
                slideTimer = slideDuration;
                slideCooldownTimer = slideCooldownDuration;
                if (keys[left]) flipMode = SDL_FLIP_HORIZONTAL;
                if (keys[right]) flipMode = SDL_FLIP_NONE;
            }
            isCrouching = true;
        }
        else {
            isCrouching = false;
        }

        bool canMove = !isCrouching && !isRolling && !isSliding && !isLightAttacking && !isHeavyAttacking;

        if (isJumping) {
            if (keys[left]) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; }
            if (keys[right]) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; }
        }
        else if (canMove) {
            if (keys[left]) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; isMoving = true; }
            else if (keys[right]) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; isMoving = true; }
        }

        if (keys[up] && !isJumping && !isRolling && !isSliding && !isCrouching) {
            velocityY = -800.0f;
            isJumping = true;
            y -= 5.0f;
        }

        if (keys[atk1] && !isLightAttacking && !isHeavyAttacking && !isRolling && !isJumping) lightAttack();
        if (keys[atk2] && !isHeavyAttacking && !isLightAttacking && !isRolling && !isJumping) heavyAttack();
    }

    void lightAttack() { isLightAttacking = true; currentFrame = 0; animationTimer = 0.0f; }
    void heavyAttack() { isHeavyAttacking = true; currentFrame = 0; animationTimer = 0.0f; }

    void update(float elapsed) {
        if (isJumping) {
            velocityY += gravity * elapsed;
            y += velocityY * elapsed;

            if (y >= groundY && velocityY > 0) {
                y = groundY;
                isJumping = false;
                isRolling = true;
                currentFrame = 0;
                animationTimer = 0.0f;
            }
        }

        if (isSliding) {
            float dir = (flipMode == SDL_FLIP_HORIZONTAL) ? -1.0f : 1.0f;
            x += (velocityX * 1.8f) * dir * elapsed;
            slideTimer -= elapsed;
            if (slideTimer <= 0) isSliding = false;
        }

        if (isRolling) {
            float dir = (flipMode == SDL_FLIP_HORIZONTAL) ? -1.0f : 1.0f;
            x += (velocityX * 0.8f) * dir * elapsed;
        }

        SpriteState nextState = SpriteState::IDLE;

        if (isDead) nextState = SpriteState::DEAD;
        else if (isLightAttacking) {
            nextState = SpriteState::ATTACK1;
            if (currentFrame >= frameCounts[nextState] - 1) isLightAttacking = false;
        }
        else if (isHeavyAttacking) {
            nextState = SpriteState::ATTACK2;
            if (currentFrame >= frameCounts[nextState] - 1) isHeavyAttacking = false;
        }
        else if (isJumping) {
            nextState = SpriteState::JUMPING;
        }
        else if (isRolling) {
            nextState = SpriteState::ROLLING;
            if (currentFrame >= frameCounts[nextState] - 1) isRolling = false;
        }
        else if (isSliding) {
            nextState = SpriteState::SLIDING;
        }
        else if (isCrouching) {
            nextState = SpriteState::CROUCH;
        }
        else if (isMoving) {
            nextState = SpriteState::WALKING;
        }

        if (currentState != nextState) {
            currentState = nextState;
            currentFrame = 0;
            animationTimer = 0.0f;
        }

        rect.x = x;
        rect.y = y;
        animate(elapsed);
    }
};