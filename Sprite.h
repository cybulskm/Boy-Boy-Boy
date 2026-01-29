#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>


class Sprite {
public:
    std::string name;
    std::string state = "ACTIVE";
    bool isJumping = false;
    std::string filepath;
    std::string flippedDirection = "RIGHT";
    float velocityY = 0.0f;
    float groundY;
    const float gravity = 1500.0f;
    float x = 0, y = 0, w = 0, h = 0, vectorX =0, vectorY = 0;
    SDL_FRect srcRect = { 0, 0, 48, 48 }; 
    int currentFrame = 0;
    int totalFrames = 6;     
    float animationTimer = 0.0f;
    float frameDuration = 0.1f; 
    bool isMoving = false;     

    SDL_Texture* texture = nullptr;
    SDL_FRect rect{};
    SDL_Renderer* renderer = nullptr;
    SDL_FlipMode flipMode = SDL_FLIP_NONE;
    SDL_Texture* walking_texture = nullptr;

    Sprite(float x, float y, float w, float h, SDL_Renderer* renderer,
        std::string path = "Sprites/3 SteamMan/SteamMan.png", std::string name="Default Sprite")
        : x(x), y(y), w(w), h(h), renderer(renderer)
    {
        this->rect = { x, y, w, h };
        this->filepath = path;
        this->name = name;
        this->groundY = y;
        texture = loadTexture(filepath);
        walking_texture = loadTexture("Sprites/3 SteamMan/SteamMan_walk.png");
    }

    SDL_Texture* loadTexture(std::string filepath) {
        SDL_Texture* newTex = IMG_LoadTexture(renderer, filepath.c_str());
        if (newTex != nullptr) {
            std::cout << "Loaded texture: " << filepath << " for character" << name << std::endl;
            return newTex;
        }
        std::cout << "Failed to load texture: " << filepath << " for character" << name << std::endl;
        
    }
   

    void animate_walking(float elapsed) {
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

    void update(float newX, float newY) {
        x = newX;
        y = newY;
        rect.x = newX;
        rect.y = newY;
    }

    void update(float elapsed) {
        animate_walking(elapsed);
        if (isJumping) {
            velocityY += gravity * elapsed;
            y += velocityY * elapsed;
            if (y >= groundY) {
                y = groundY;
                isJumping = false;
                velocityY = 0;
            }
        }
        rect.x = x;
        rect.y = y;
    }
    void toggleFlip() {
        if (flipMode == SDL_FLIP_NONE) {
            flipMode = SDL_FLIP_HORIZONTAL;
        }
        else {
            flipMode = SDL_FLIP_NONE;
        }
        std::cout << "Flipped to " << flipMode << std::endl;
    }

    void flipLeft() {
        flipMode = SDL_FLIP_HORIZONTAL;
    }
    void flipRight() {
        flipMode = SDL_FLIP_NONE;
    }
    void handleInput(const bool* keys, float speed, const float elapsed) {
        isMoving = false; // Assume we are standing still this frame

        if (keys[SDL_SCANCODE_LEFT]) {
            walkLeft(speed, elapsed);
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            walkRight(speed, elapsed);
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_UP]) jump();
    }
    void walkLeft(float speed, float elapsed) {
        x -= speed * elapsed;
        flipLeft(); 
    }

    void walkRight(float speed, float elapsed) {
        x += speed * elapsed;
        flipRight(); 
    }
    void jump() {
        if (!isJumping) {
            velocityY = -600.0f; 
            isJumping = true;
        }
    }

    void draw() {
        if (state == "ACTIVE") {
            // Pick which texture to use
            SDL_Texture* currentTex = (isMoving) ? walking_texture : texture;

            if (currentTex) {
                // Passing &srcRect instead of NULL is the magic step
                SDL_RenderTextureRotated(renderer, currentTex, &srcRect, &rect, 0.0, NULL, flipMode);
            }
        }
    }

    virtual ~Sprite() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
};
