#include "SpriteRenderer.h"

void SpriteRenderer::draw(SDL_Renderer* renderer, SDL_Texture* texture,
                          const SDL_Rect* srcRect, const SDL_Rect* dstRect,
                          double angle, SDL_Point* point, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, angle, point, flip);
}
