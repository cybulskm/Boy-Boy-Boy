#pragma once
#include <SDL3/SDL.h>
#include <map>
#include <string>
#include <iostream>
#include "SpriteState.h"
#include "CharacterData.h"
#include "TextureCache.h"

class Sprite {
public:
    float x = 0, y = 0, originalw = 0, originalh = 0, draww = 0, drawh = 0;
    SDL_FRect rect{};
    SDL_FRect srcRect = { 0, 0, 0, 0 };
    Uint8 modR = 255, modG = 255, modB = 255;
    int currentFrame = 0;
    float animationTimer = 0.0f;
    float frameDuration = 0.1f;
    float velocityY = 0.0f;
    float velocityX = 0.0f;
    float hitboxOffsetX = 0, hitboxOffsetY = 0;
    float hitboxW = 0, hitboxH = 0;
    int hitCount = 0;
    SpriteState currentState = SpriteState::IDLE;
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
    SDL_Renderer* renderer = nullptr;
    TextureCache* globalCacheRef = nullptr; // For GIF lookup

    std::map<SpriteState, SDL_Texture*> animations;
    std::map<SpriteState, std::string> animationNames;
    std::map<SpriteState, int> frameCounts;

    bool canCollide = true;
    bool debugMode = false;
    std::string name = "Unnamed Sprite";

    // RESTORED: 7-argument constructor for Object.h and UI.h
    Sprite(float x, float y, float originalw, float originalh, float draww, float drawh, SDL_Renderer* renderer)
        : x(x), y(y), originalw(originalw), originalh(originalh), draww(draww), drawh(drawh), renderer(renderer) {
        rect = { x, y, draww, drawh };
        srcRect = { 0.0f, 0.0f, originalw, originalh };
    }

    // Constructor for Player/CharacterData
    Sprite(float x, float y, const CharacterData& charData, SDL_Renderer* renderer)
        : x(x), y(y), originalw(charData.originalw), originalh(charData.originalh),
        draww(charData.draww), drawh(charData.drawh), renderer(renderer) {

        this->rect = { x, y, draww, drawh };

        // Use custom data if it exists, otherwise use the 40%/80% default
        this->hitboxW = (charData.hitboxW > 0) ? charData.hitboxW : draww * 0.4f;
        this->hitboxH = (charData.hitboxH > 0) ? charData.hitboxH : drawh * 0.8f;
        this->hitboxOffsetX = (charData.offsetX > 0) ? charData.offsetX : (draww - hitboxW) / 2.0f;
        this->hitboxOffsetY = (charData.offsetY > 0) ? charData.offsetY : (drawh - hitboxH);

        this->animations = charData.animations;
        this->animationNames = charData.animKeys;
        this->frameCounts = charData.frameCounts;
        this->velocityX = charData.velocityX;
    }

    virtual void update(float elapsed) {
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
    }

    // FIXED: draw() now takes 0 arguments for the main loop
    void draw() {
        SDL_Texture* tex = nullptr;
        std::string key = "";

        // 1. Try to get GIF frame from cache
        if (animationNames.count(currentState)) {
            key = animationNames[currentState];
            if (globalCacheRef) tex = globalCacheRef->get(key, currentFrame);
        }

        // 2. Fallback to static texture (Spritesheet/Object/UI)
        if (!tex && animations.count(currentState)) {
            tex = animations[currentState];
        }

        if (tex) {
            SDL_FRect sRect = { 0, 0, originalw, originalh };
            bool isGif = false;
            if (globalCacheRef && !key.empty()) {
                if (globalCacheRef->getCount(key, 0) > 1) isGif = true;
            }

            // If not a GIF, do the spritesheet slicing
            if (!isGif) sRect.x = (float)currentFrame * originalw;

            if (modR != 255 || modG != 255 || modB != 255) SDL_SetTextureColorMod(tex, modR, modG, modB);
            SDL_RenderTextureRotated(renderer, tex, &sRect, &rect, 0.0, NULL, flipMode);
            if (modR != 255 || modG != 255 || modB != 255) SDL_SetTextureColorMod(tex, 255, 255, 255);
        }
        if (debugMode) showHitBox();
    }

    // RESTORED: getBounds()
    SDL_FRect getBounds() const { return rect; }


    bool intersects(const SDL_FRect& a, const SDL_FRect& b) {
        return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
    }
    // Helper to get the actual physical box
    virtual SDL_FRect getHitbox()  const{
        return { x + hitboxOffsetX, y + hitboxOffsetY, hitboxW, hitboxH };
    }

    void showHitBox() {
        SDL_FRect hb = getHitbox();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderRect(renderer, &hb); // Draw the REAL box
    }

    bool intersects(Sprite* other) {
        if (!other) return false;
        SDL_FRect a = getHitbox();
        SDL_FRect b = other->getHitbox();
        return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
    }

    void setColour(Uint8 r, Uint8 g, Uint8 b) { modR = r; modG = g; modB = b; }
    virtual void onCollision(Sprite* other) {}
    virtual void handleInput(const bool* keys, float elapsed) {} // Added for unified sprite processing
    virtual ~Sprite() {}
};