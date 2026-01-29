#pragma once
#include "InputComponent.h"
class PlayerInput : public InputComponent {
public:
    void update(Sprite& sprite, float elapsed) override {
        const bool* keys = SDL_GetKeyboardState(NULL);
        float speed = 300.0f;

        sprite.isMoving = false;
        if (keys[SDL_SCANCODE_LEFT]) {
            sprite.walkLeft(speed, elapsed);
            sprite.isMoving = true;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            sprite.walkRight(speed, elapsed);
            sprite.isMoving = true;
        }
        if (keys[SDL_SCANCODE_SPACE]) { // Space to jump
            sprite.jump();
        }
    }
};