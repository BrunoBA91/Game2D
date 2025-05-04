#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

enum class EntityType { None, Player, Enemy, Wall, Item };

class Entity {
public:
    Entity();
    virtual ~Entity();

    virtual bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h);
    virtual void update(const std::vector<SDL_Rect>& walls, const std::vector<Entity*>& others) = 0;
    virtual void render(SDL_Renderer* renderer);
    virtual void clean();

    void setFlip(SDL_RendererFlip f) { flip = f; }
    SDL_RendererFlip getFlip() const { return flip; }

    void setOrigin(int x, int y) { origin = {x,y}; }

    virtual EntityType getType() const { return EntityType::None; }

    const SDL_Rect& getRect() const { return rect; }

protected:
    SDL_Texture* texture;
    SDL_Rect rect;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_Point origin = {0,0};
};