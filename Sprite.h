#pragma once
#include <SDL3/SDL.h>
#include <map>
#include <string>

enum class SpriteState { IDLE, WALKING, JUMPING, ATTACKING };

class Sprite {
public:
    float x = 0, y = 0, originalw = 0, originalh = 0, draww = 0, drawh = 0;
    SDL_FRect rect{};
    SDL_FRect srcRect = { 0, 0, 0, 0 };
    Uint8 modR = 255, modG = 255, modB = 255;
    int currentFrame = 0;
    float animationTimer = 0.0f;
    float frameDuration = 0.1f;

    SpriteState currentState = SpriteState::IDLE;
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
    SDL_Renderer* renderer = nullptr;

    std::map<SpriteState, SDL_Texture*> animations;
    std::map<SpriteState, int> frameCounts;


    Sprite(float x, float y, float originalw, float originalh, float draww, float drawh, SDL_Renderer* renderer)
        : x(x), y(y), originalh(originalh), originalw(originalw), draww(draww), drawh(drawh), renderer(renderer) {
        rect = { x, y, originalw, originalh };
		srcRect = { 0.0f, 0.0f, originalw, originalh };
    }

    void animate(float elapsed) {
        int totalFrames = frameCounts[currentState];
        if (totalFrames <= 0) return;

        animationTimer += elapsed;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.0f;
            currentFrame = (currentFrame + 1) % totalFrames;
        }
        srcRect.x = (float)currentFrame * srcRect.w;
    }
    void modifyRender(Uint8 r, Uint8 g, Uint8 b) {
		SDL_SetTextureColorMod(animations[currentState], r, g, b);
    }

    void draw() {
        SDL_Texture* tex = animations[currentState];
        if (tex) {
            SDL_SetTextureColorMod(tex, modR, modG, modB);
            SDL_RenderTextureRotated(renderer, tex, &srcRect, &rect, 0.0, NULL, flipMode);
            SDL_SetTextureColorMod(tex, 255, 255, 255);
        }
    }
    void setColour(Uint8 r, Uint8 g, Uint8 b) {
        modR = r;
        modG = g;
        modB = b;
    }

    virtual ~Sprite() {}
};