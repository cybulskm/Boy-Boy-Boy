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

    SDL_Gamepad* gamepad = nullptr;
    Player(float x, float y, const CharacterData& charData, SDL_Renderer* renderer)
        : Sprite(x, y, charData, renderer)
    {
        this->name = charData.name;
        this->groundY = 900;
        this->currentState = SpriteState::IDLE;
        this->baseAttackIncrease = charData.attackHitboxIncrease;

        // SDL3 Gamepad opening logic
        int count;
        SDL_JoystickID* gamepads = SDL_GetGamepads(&count);
        if (gamepads && playerID < count) {
            gamepad = SDL_OpenGamepad(gamepads[playerID]);
        }
        SDL_free(gamepads);
    }
    bool isPressed(const bool* keys, SDL_Scancode key, SDL_GamepadButton btn) {
        bool keyboard = keys[key];
        bool pad = (gamepad && SDL_GetGamepadButton(gamepad, btn));
        return keyboard || pad;
    }
    void handleInput(const bool* keys, float elapsed) {
        if (isHurt || isDead) return;
        isMoving = false;

        SDL_Scancode kUp = (playerID == 0) ? SDL_SCANCODE_W : SDL_SCANCODE_UP;
        SDL_Scancode kLeft = (playerID == 0) ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT;
        SDL_Scancode kRight = (playerID == 0) ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT;
        SDL_Scancode kDown = (playerID == 0) ? SDL_SCANCODE_S : SDL_SCANCODE_DOWN;
        SDL_Scancode kAtk1 = (playerID == 0) ? SDL_SCANCODE_1 : SDL_SCANCODE_N;
        SDL_Scancode kAtk2 = (playerID == 0) ? SDL_SCANCODE_2 : SDL_SCANCODE_M;

        // SDL3 Button Names
        bool pressUp = isPressed(keys, kUp, SDL_GAMEPAD_BUTTON_DPAD_UP);
        bool pressDown = isPressed(keys, kDown, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
        bool pressLeft = isPressed(keys, kLeft, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
        bool pressRight = isPressed(keys, kRight, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
        bool pressAtk1 = isPressed(keys, kAtk1, SDL_GAMEPAD_BUTTON_SOUTH); // Usually 'A'
        bool pressAtk2 = isPressed(keys, kAtk2, SDL_GAMEPAD_BUTTON_WEST);  // Usually 'X'

        if (slideCooldownTimer > 0) slideCooldownTimer -= elapsed;

        if (isGrounded && pressDown) {
            if ((pressLeft || pressRight) && !isSliding && slideCooldownTimer <= 0) {
                isSliding = true;
                isCrouching = false;
                slideTimer = slideDuration;
                slideCooldownTimer = slideCooldownDuration;
                if (pressLeft) flipMode = SDL_FLIP_HORIZONTAL;
                if (pressRight) flipMode = SDL_FLIP_NONE;
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
            if (pressLeft) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; }
            if (pressRight) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; }
        }
        else if (canMove) {
            if (pressLeft) { x -= velocityX * elapsed; flipMode = SDL_FLIP_HORIZONTAL; isMoving = true; }
            else if (pressRight) { x += velocityX * elapsed; flipMode = SDL_FLIP_NONE; isMoving = true; }
        }

        if (pressUp && isGrounded && !isSliding && !isCrouching) {
            velocityY = -800.0f;
            isGrounded = false;
            isJumping = true;
        }

        if (pressAtk1 && !isLightAttacking && !isHeavyAttacking && !isSliding) lightAttack();
        if (pressAtk2 && !isHeavyAttacking && !isLightAttacking && !isSliding) heavyAttack();
    }

    ~Player() {
        if (gamepad) SDL_CloseGamepad(gamepad);
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
    void hurt(float elapsed) {
        hurtTimer -= elapsed;
        setColour(255, 0, 0);
        if (hurtTimer <= 0) {
            hitCount += 1;
            isHurt = false;
            setColour(255, 255, 255);
        }
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
            hurt(elapsed);
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