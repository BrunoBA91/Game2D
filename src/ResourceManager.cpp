#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "ResourceManager.h"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    clear(); // Ensure cleanup
}

void ResourceManager::loadTexture(const std::string& id, const std::string& path, SDL_Renderer* renderer) {
    if (textures.find(id) != textures.end()) {
        std::cout << "[ResourceManager] Texture already loaded: " << id << std::endl;
        return;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "[ResourceManager] Failed to load surface: " << path << " - " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "[ResourceManager] Failed to create texture: " << path << " - " << SDL_GetError() << std::endl;
        return;
    }

    textures[id] = texture;
}

SDL_Texture* ResourceManager::getTexture(const std::string& id) const {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }

    std::cerr << "[ResourceManager] Texture not found: " << id << std::endl;
    return nullptr;
}

void ResourceManager::clear() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}
