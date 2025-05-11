#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

enum class EntityType { None, Player, Enemy, Wall, Item };

class Entity {
public:
    virtual ~Entity() = default;

    virtual bool init(SDL_Renderer* renderer,
                      const std::string& imagePath,
                      int x, int y, int w, int h) = 0;

    virtual void update(const std::vector<SDL_Rect>& walls,
                        const std::vector<Entity*>& others,
                        float deltaTime) = 0;

    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void clean() = 0;

    virtual EntityType getType() const { return EntityType::None; }

    virtual void setFlip(SDL_RendererFlip f) = 0;
    virtual SDL_RendererFlip getFlip() const = 0;
};
