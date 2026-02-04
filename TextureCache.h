#pragma once
#include <map>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

class TextureCache {
public:
    SDL_Renderer* renderer = nullptr;
    TextureCache(SDL_Renderer* renderer) : renderer(renderer) {}

    void load(const std::string& name, const std::string& filePath) {
        if (cache.find(name) != cache.end()) return;

        SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
        if (!tex) {
            // THIS LINE IS CRUCIAL:
            std::cerr << "SDL Error: " << SDL_GetError() << " | Path: " << filePath << std::endl;
            return;
        }
        cache[name] = tex;
    }

    SDL_Texture* get(const std::string& name) {
        auto it = cache.find(name);
        if (it == cache.end()) {
            std::cerr << "Warning: Texture '" << name << "' not found in cache!" << std::endl;
            return nullptr;
        }
        return it->second;
    }

    void clear() {
        for (auto const& [name, tex] : cache) {
            SDL_DestroyTexture(tex);
        }
        cache.clear();
    }

private:
    std::map<std::string, SDL_Texture*> cache;
};