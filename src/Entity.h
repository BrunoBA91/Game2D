#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Entity {
public:
    Entity();
    virtual ~Entity();

    virtual bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h);
    virtual void update(); // can be overridden
    virtual void render(SDL_Renderer* renderer);
    virtual void clean();

    void setFlip(SDL_RendererFlip f) { flip = f; }
    SDL_RendererFlip getFlip() const { return flip; }

protected:
    SDL_Texture* texture;
    SDL_Rect rect;

    SDL_RendererFlip flip = SDL_FLIP_NONE;
};