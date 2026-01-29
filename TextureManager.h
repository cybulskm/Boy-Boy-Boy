#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <map>
#include "TextureName.h"
using namespace std;


class TextureManager {
private:
    map<TextureName, SDL_Texture*> textureMap;
    SDL_Renderer* renderer;

public:
    TextureManager(SDL_Renderer* ren) : renderer(ren) {}

    SDL_Texture* getTexture(TextureName name, const std::string& path) {
        // If it's already loaded, just return it
        if (textureMap.count(name)) {
            return textureMap[name];
        }

        // Otherwise, load it now
        SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
        if (tex) {
            textureMap[name] = tex;
            std::cout << "Successfully cached: " << path << std::endl;
        }
        return tex;
    }

    ~TextureManager() {
        for (auto const& [name, tex] : textureMap) {
            SDL_DestroyTexture(tex);
        }
    }
};