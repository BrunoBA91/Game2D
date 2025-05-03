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
    virtual void render(SDL_Renderer* renderer) const;
    virtual void clean();

protected:
    SDL_Texture* texture;
    SDL_Rect rect;
};