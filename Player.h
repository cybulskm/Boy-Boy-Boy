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
    bool isGrounded = false;
    float groundY;
    const float gravity = 1500.0f;
    float slideTimer = 0.0f;
    const float slideDuration = 0.5f;

    float slideCooldownTimer = 0.0f;
    const float slideCooldownDuration = 1.2f;

    float hurtTimer = 0.0f;
    const float hurtDuration = 1.5f;

    float attackIncreaseValue = 0.0f;
    float baseAttackIncrease = 0.0f;

    Player(float x, float y, const CharacterData& charData, SDL_Renderer* renderer)
        : Sprite(x, y, charData, renderer)
    {
        this->name = charData.name;
        this->groundY = 1500;
        this->currentState = SpriteState::IDLE;
        this->baseAttackIncrease = charData.attackHitboxIncrease;
    }

    void handleInput(const bool* keys, float elapsed) {
        if (isHurt || isDead) return;

        isMoving = false;

        SDL_Scancode up = (playerID == 0) ? SDL_SCANCODE_W : SDL_SCANCODE_UP;
        SDL_Scancode left = (playerID == 0) ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT;
        SDL_Scancode right = (playerID == 0) ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT;
        SDL_Scancode down = (playerID == 0) ? SDL_SCANCODE_S : SDL_SCANCODE_DOWN;
        SDL_Scancode atk1 = (playerID == 0) ? SDL_SCANCODE_1 : SDL_SCANCODE_N;
        SDL_Scancode atk2 = (playerID == 0) ? SDL_SCANCODE_2 : SDL_SCANCODE_M;

        if (slideCooldownTimer > 0) slideCooldownTimer -= elapsed;

        if (isGrounded && keys[down]) {
            if ((keys[left] || keys[right]) && !isSliding && slideCooldownTimer <= 0) {
                isSliding = true;
                isCrouching = false;
                slideTimer = slideDuration;
                slideCooldownTimer = slideCooldownDuration;
            }
            else if (!isSliding) {
                isCrouching = true;
            }
        }
        else {
            isCrouching = false;
        }

        bool canMove = !isCrouching && !isSliding && !isLightAttacking && !isHeavyAttacking;

        if (!isGrounded) {
            if (keys[left]) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; }
            if (keys[right]) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; }
        }
        else if (canMove) {
            if (keys[left]) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; isMoving = true; }
            else if (keys[right]) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; isMoving = true; }
        }

        if (keys[up] && isGrounded && !isSliding && !isCrouching) {
            velocityY = -800.0f;
            isGrounded = false;
            isJumping = true;
        }

        if (keys[atk1] && !isLightAttacking && !isHeavyAttacking && !isSliding) lightAttack();
        if (keys[atk2] && !isHeavyAttacking && !isLightAttacking && !isSliding) heavyAttack();
    }

    void lightAttack() {
        if (!isLightAttacking && !isHeavyAttacking) {
            isLightAttacking = true;
            currentFrame = 0;
            animationTimer = 0.0f;
            attackIncreaseValue = baseAttackIncrease;
        }
    }

    void heavyAttack() {
        if (!isHeavyAttacking && !isLightAttacking) {
            isHeavyAttacking = true;
            currentFrame = 0;
            animationTimer = 0.0f;
            attackIncreaseValue = baseAttackIncrease;
        }
    }

    SDL_FRect getHitbox() const override {
        SDL_FRect hb;
        hb.w = hitboxW + attackIncreaseValue;
        hb.h = (isCrouching || isSliding) ? hitboxH * 0.5f : hitboxH;
        hb.x = (flipMode == SDL_FLIP_NONE) ? x + hitboxOffsetX : x + hitboxOffsetX - attackIncreaseValue;
        hb.y = (isCrouching || isSliding) ? y + hitboxOffsetY + (hitboxH * 0.5f) : y + hitboxOffsetY;
        return hb;
    }

    void update(float elapsed) {
        // Apply gravity constantly
        velocityY += gravity * elapsed;
        y += velocityY * elapsed;


        if (y >= groundY) {
            y = groundY;
            velocityY = 0;
            isGrounded = true;
        }

        if (isSliding) {
            float dir = (flipMode == SDL_FLIP_HORIZONTAL) ? -1.0f : 1.0f;
            x += (velocityX * 1.8f) * dir * elapsed;
            slideTimer -= elapsed;
            if (slideTimer <= 0) isSliding = false;
        }

        if (isHurt) {
            hurtTimer -= elapsed;
            if (hurtTimer <= 0) {
                isHurt = false;
                setColour(255, 255, 255);
            }
        }

        SpriteState nextState = SpriteState::IDLE;
        if (isDead) nextState = SpriteState::DEAD;
        else if (isHurt) nextState = SpriteState::HURT;
        else if (isLightAttacking) {
            nextState = SpriteState::ATTACK1;
            if (currentFrame >= frameCounts[nextState] - 1) { isLightAttacking = false; attackIncreaseValue = 0; }
        }
        else if (isHeavyAttacking) {
            nextState = SpriteState::ATTACK2;
            if (currentFrame >= frameCounts[nextState] - 1) { isHeavyAttacking = false; attackIncreaseValue = 0; }
        }
        else if (isSliding) nextState = SpriteState::SLIDING;
        else if (!isGrounded) nextState = SpriteState::JUMPING;
        else if (isCrouching) nextState = SpriteState::CROUCH;
        else if (isMoving) nextState = SpriteState::WALKING;

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