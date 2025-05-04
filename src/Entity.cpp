#include "Entity.h"
#include <iostream>

Entity::Entity() : texture(nullptr), rect{0, 0, 0, 0} {}

Entity::~Entity() {
    clean();
}

bool Entity::init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << "\n";
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
        return false;
    }

    rect = { x, y, w, h };
    return true;
}

void Entity::update() {
    // base implementation: do nothing
}

void Entity::render(SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, 0.0, &origin, getFlip());
}

void Entity::clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}