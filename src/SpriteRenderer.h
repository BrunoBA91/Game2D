#pragma once
#include <SDL2/SDL.h>

class SpriteRenderer {
public:
    static void draw(SDL_Renderer* renderer, SDL_Texture* texture,
                     const SDL_Rect* srcRect, const SDL_Rect* dstRect,
                     double angle, SDL_Point* point, SDL_RendererFlip flip = SDL_FLIP_NONE);
};
