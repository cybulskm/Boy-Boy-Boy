#pragma once
#include <SDL3/SDL.h>
#include <map>
#include <string>
#include "SpriteState.h"
#include <iostream>

class Sprite {
public:
    float x = 0, y = 0, originalw = 0, originalh = 0, draww = 0, drawh = 0;
    SDL_FRect rect{}; // This IS your hitbox
    SDL_FRect srcRect = { 0, 0, 0, 0 };
    Uint8 modR = 255, modG = 255, modB = 255;
    int currentFrame = 0;
    float animationTimer = 0.0f;
    float frameDuration = 0.1f;
    float velocityY = 0.0f;
    float velocityX = 0.0f;
    SpriteState currentState = SpriteState::IDLE;
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
    SDL_Renderer* renderer = nullptr;

    std::map<SpriteState, SDL_Texture*> animations;
    std::map<SpriteState, int> frameCounts;

    bool canCollide = true;
    bool debugMode = false;
    std::string name = "Unnamed Sprite";

    Sprite(float x, float y, float originalw, float originalh, float draww, float drawh, SDL_Renderer* renderer)
        : x(x), y(y), originalh(originalh), originalw(originalw), draww(draww), drawh(drawh), renderer(renderer) {
        rect = { x, y, draww, drawh };
        srcRect = { 0.0f, 0.0f, originalw, originalh };
    }

    // Call this in your update loop to keep the hitbox synced with x and y
    void update(float elapsed) {
        animate(elapsed);
        rect.x = x;
        rect.y = y;
    }

    void animate(float elapsed) {
        int totalFrames = frameCounts[currentState];
        if (totalFrames <= 0) return;

        animationTimer += elapsed;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.0f;
            if (currentState == SpriteState::ATTACK1 || currentState == SpriteState::ATTACK2) {
                if (currentFrame < totalFrames - 1) currentFrame++;
            }
            else {
                currentFrame = (currentFrame + 1) % totalFrames;
            }
        }
        srcRect.x = (float)currentFrame * originalw;
    }

    void draw() {
        SDL_Texture* tex = animations[currentState];
        if (tex) {
            if (modR != 255 || modG != 255 || modB != 255) {
                SDL_SetTextureColorMod(tex, modR, modG, modB);
            }
            SDL_RenderTextureRotated(renderer, tex, &srcRect, &rect, 0.0, NULL, flipMode);
            if (modR != 255 || modG != 255 || modB != 255) {
                SDL_SetTextureColorMod(tex, 255, 255, 255);
            }
        }

        // DRAW HITBOX
        if (debugMode) {
            showHitBox();
        }
    }

    void showHitBox() {
		std::cout << "Debug enabled for  << name << " << name << std::endl;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderRect(renderer, &rect); // Outlines the current rect
    }

    virtual void onCollision(Sprite* other) {
        std::cout << name << " hit " << other->name << std::endl;
    }

    void setColour(Uint8 r, Uint8 g, Uint8 b) {
        modR = r;
        modG = g;
        modB = b;
    }

    bool intersects(const SDL_FRect& a, const SDL_FRect& b) {
        return a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y;
    }

    SDL_FRect getBounds() const {
        return rect; // Just return the rect we are already updating
    }

    virtual ~Sprite() {}
};