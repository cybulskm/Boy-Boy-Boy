#pragma once
#include <map>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>

class TextureCache {
public:
    SDL_Renderer* renderer = nullptr;
    TextureCache(SDL_Renderer* renderer) : renderer(renderer) {}

    void load(const std::string& name, const std::string& filePath) {
        if (cache.count(name)) return;

        std::string ext = "";
        size_t dotPos = filePath.find_last_of(".");
        if (dotPos != std::string::npos) {
            ext = filePath.substr(dotPos + 1);
            for (char& c : ext) {
                c = std::tolower((unsigned char)c);
            }
        }

        if (ext == "gif") {
            IMG_Animation* anim = IMG_LoadAnimation(filePath.c_str());
            if (anim) {
                std::vector<SDL_Texture*> frames;
                for (int i = 0; i < anim->count; ++i) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, anim->frames[i]);
                    if (tex) frames.push_back(tex);
                }
                cache[name] = frames;
                IMG_FreeAnimation(anim);
                return; // Successfully loaded animated GIF
            }
            std::cerr << "IMG_LoadAnimation failed, falling back to texture: " << SDL_GetError() << " | Path: " << filePath << std::endl;
        }
        // Fallback for non-GIF formats or if IMG_LoadAnimation failed
        SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
        if (tex) cache[name] = { tex };
    }

    SDL_Texture* get(const std::string& name, int frameIndex = 0) {
        if (cache.find(name) == cache.end()) return nullptr;
        const auto& frames = cache.at(name);
        if (frames.empty()) return nullptr;
        return frames[frameIndex % frames.size()];
    }

    int getCount(const std::string& name, int defaultCount) {
        if (cache.count(name) && cache.at(name).size() > 1) {
            return (int)cache.at(name).size();
        }
        return defaultCount;
    }

    void clear() {
        for (auto& [name, frames] : cache) {
            for (auto* tex : frames) SDL_DestroyTexture(tex);
        }
        cache.clear();
    }

private:
    std::map<std::string, std::vector<SDL_Texture*>> cache;
};